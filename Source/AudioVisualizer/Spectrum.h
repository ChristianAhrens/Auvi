//
//  Spectrum.h
//  3DAudioVisualizers
//
//  Created by Tim Arterbury on 5/3/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "RingBuffer.h"

#define RING_BUFFER_READ_SIZE 256

/** Frequency Spectrum visualizer. Uses basic shaders, and calculates all points
    on the CPU as opposed to the OScilloscope3D which calculates points on the
    GPU.
 */

class Spectrum :    public Component,
                    public OpenGLRenderer
{
    
public:
    Spectrum (RingBuffer<GLfloat> * ringBuffer)
    :   m_readBuffer (2, RING_BUFFER_READ_SIZE),
        m_forwardFFT(fftOrder)
    {
        // Sets the version to 3.2
        m_openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
     
        m_ringBuffer = ringBuffer;
        
        // Set default 3D orientation
        m_draggableOrientation.reset(Vector3D<float>(0.0, 1.0, 0.0));
        
        // Allocate FFT data
        m_fftData = new GLfloat [2 * fftSize];
        
        // Attach the OpenGL context but do not start [ see start() ]
        m_openGLContext.setRenderer(this);
        m_openGLContext.attachTo(*this);
        
        // Setup GUI Overlay Label: Status of Shaders, compiler errors, etc.
        addAndMakeVisible (m_statusLabel);
        m_statusLabel.setJustificationType (Justification::topLeft);
        m_statusLabel.setFont (Font (14.0f));
    }
    
    ~Spectrum()
    {
        // Turn off OpenGL
        m_openGLContext.setContinuousRepainting (false);
        m_openGLContext.detach();
        
        delete [] m_fftData;
        
        // Detach ringBuffer
        m_ringBuffer = nullptr;
    }
    
    //==========================================================================
    // Oscilloscope Control Functions
    
    void start()
    {
        m_openGLContext.setContinuousRepainting (true);
    }
    
    void stop()
    {
        m_openGLContext.setContinuousRepainting (false);
    }
    
    
    //==========================================================================
    // OpenGL Callbacks
    
    /** Called before rendering OpenGL, after an OpenGLContext has been associated
        with this OpenGLRenderer (this component is a OpenGLRenderer).
        Sets up GL objects that are needed for rendering.
     */
    void newOpenGLContextCreated() override
    {
        // Setup Sizing Variables
        m_xFreqWidth = 3.0f;
        m_yAmpHeight = 1.0f;
        m_zTimeDepth = 3.0f;
        m_xFreqResolution = 50;
        m_zTimeResolution = 60;

        m_numVertices = m_xFreqResolution * m_zTimeResolution;
        
        // Initialize XZ Vertices
        initializeXZVertices();
        
        // Initialize Y Vertices
        initializeYVertices();
        
        // Setup Buffer Objects
        m_openGLContext.extensions.glGenBuffers (1, &m_xzVBO); // Vertex Buffer Object
        m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_xzVBO);
        m_openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numVertices * 2, m_xzVertices, GL_STATIC_DRAW);
        
        
        m_openGLContext.extensions.glGenBuffers (1, &m_yVBO);
        m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_yVBO);
        m_openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numVertices, m_yVertices, GL_STREAM_DRAW);
        
#ifdef JUCE_OPENGL3
        m_openGLContext.extensions.glGenVertexArrays(1, &VAO);
        m_openGLContext.extensions.glBindVertexArray(VAO);
#endif
        m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_xzVBO);
        m_openGLContext.extensions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
        m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_yVBO);
        m_openGLContext.extensions.glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), NULL);
        
        m_openGLContext.extensions.glEnableVertexAttribArray (0);
        m_openGLContext.extensions.glEnableVertexAttribArray (1);
        
        glPointSize (6.0f);
        
        // Setup Shaders
        createShaders();
    }
    
    /** Called when done rendering OpenGL, as an OpenGLContext object is closing.
        Frees any GL objects created during rendering.
     */
    void openGLContextClosing() override
    {
        if(m_shader)
            m_shader->release();
        m_shader = nullptr;
        m_uniforms = nullptr;
        
        delete [] m_xzVertices;
        delete [] m_yVertices;
    }
    
    
    /** The OpenGL rendering callback.
     */
    void renderOpenGL() override
    {
        jassert (OpenGLHelpers::isContextActive());
        if (!m_shader)
            return;
        
        // Setup Viewport
        const float renderingScale = (float)m_openGLContext.getRenderingScale();
        glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
        
        // Set background Color
        OpenGLHelpers::clear (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
        // Enable Alpha Blending
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Use Shader Program that's been defined
        m_shader->use();
        
        // Copy data from ring buffer into FFT
        m_ringBuffer->readSamples (m_readBuffer, RING_BUFFER_READ_SIZE);
        FloatVectorOperations::clear (m_fftData, RING_BUFFER_READ_SIZE);
        
        /** Future Feature:
            Instead of summing channels below, keep the channels seperate and
            lay out the spectrum so you can see the left and right channels
            individually on either half of the spectrum.
         */
        // Sum channels together
        for (int i = 0; i < 2; ++i)
        {
            FloatVectorOperations::add (m_fftData, m_readBuffer.getReadPointer(i, 0), RING_BUFFER_READ_SIZE);
        }
        
        // Calculate FFT Crap
        m_forwardFFT.performFrequencyOnlyForwardTransform (m_fftData);
        
        // Find the range of values produced, so we can scale our rendering to
        // show up the detail clearly
        Range<float> maxFFTLevel = FloatVectorOperations::findMinAndMax (m_fftData, fftSize / 2);
        
        // Calculate new y values and shift old y values back
        for (int i = m_numVertices; i >= 0; --i)
        {
            // For the first row of points, render the new height via the FFT
            if (i < m_xFreqResolution)
            {
                const float skewedProportionY = 1.0f - std::exp (std::log (i / ((float)m_xFreqResolution - 1.0f)) * 0.2f);
                const int fftDataIndex = jlimit (0, fftSize / 2, (int) (skewedProportionY * fftSize / 2));
                float level = 0.0f;
                
                if (maxFFTLevel.getEnd() != 0.0f)
                    level = jmap (m_fftData[fftDataIndex], 0.0f, maxFFTLevel.getEnd(), 0.0f, m_yAmpHeight);
                
                m_yVertices[i] = level;
            }
            else // For the subsequent rows, shift back
            {
                m_yVertices[i] = m_yVertices[i - m_xFreqResolution];
            }
        }
        
        m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_yVBO);
        m_openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numVertices, m_yVertices, GL_STREAM_DRAW);
        
        
        // Setup the Uniforms for use in the Shader
        if (m_uniforms->projectionMatrix != nullptr)
            m_uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);
        
        if (m_uniforms->viewMatrix != nullptr)
        {
            Matrix3D<float> scale;
            scale.mat[0] = 2.0;
            scale.mat[5] = 2.0;
            scale.mat[10] = 2.0;
            Matrix3D<float> finalMatrix = scale * getViewMatrix();
            m_uniforms->viewMatrix->setMatrix4 (finalMatrix.mat, 1, false);
            
        }

        // Draw the points
#ifdef JUCE_OPENGL3
        openGLContext.extensions.glBindVertexArray(VAO);
#endif
        glDrawArrays (GL_POINTS, 0, m_numVertices);
        
        
        // Zero Out FFT for next use
        zeromem (m_fftData, sizeof (GLfloat) * 2 * fftSize);
        
        // Reset the element buffers so child Components draw correctly
//        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
//        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
//        openGLContext.extensions.glBindVertexArray(0);
    }
    
    
    //==========================================================================
    // JUCE Callbacks
    
    void paint (Graphics& g) override {}
    
    void resized () override
    {
        m_draggableOrientation.setViewport (getLocalBounds());
        m_statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
    }
    
    void mouseDown (const MouseEvent& e) override
    {
        m_draggableOrientation.mouseDown (e.getPosition());
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        m_draggableOrientation.mouseDrag (e.getPosition());
    }
    
private:
    
    //==========================================================================
    // Mesh Functions
    
    // Initialize the XZ values of vertices
    void initializeXZVertices()
    {
        
        int numFloatsXZ = m_numVertices * 2;
        
        m_xzVertices = new GLfloat [numFloatsXZ];
        
        // Variables when setting x and z
        int numFloatsPerRow = m_xFreqResolution * 2;
        GLfloat xOffset = m_xFreqWidth / ((GLfloat) m_xFreqResolution - 1.0f);
        GLfloat zOffset = m_zTimeDepth / ((GLfloat) m_zTimeResolution - 1.0f);
        GLfloat xStart = -(m_xFreqWidth / 2.0f);
        GLfloat zStart = -(m_zTimeDepth / 2.0f);
        
        // Set all X and Z values
        for (int i = 0; i < numFloatsXZ; i += 2)
        {
            
            int xFreqIndex = (i % (numFloatsPerRow)) / 2;
            int zTimeIndex = floor (i / numFloatsPerRow);
            
            // Set X Vertex
            m_xzVertices[i] = xStart + xOffset * xFreqIndex;
            m_xzVertices[i + 1] = zStart + zOffset * zTimeIndex;
        }
    }
    
    // Initialize the Y valies of vertices
    void initializeYVertices()
    {
        // Set all Y values to 0.0
        m_yVertices = new GLfloat [m_numVertices];
        memset(m_yVertices, 0.0f, sizeof(GLfloat) * m_xFreqResolution * m_zTimeResolution);
    }
    
    
    //==========================================================================
    // OpenGL Functions
    
    /** Calculates and returns the Projection Matrix.
     */
    Matrix3D<float> getProjectionMatrix() const
    {
        float w = 1.0f / (0.5f + 0.1f);
        float h = w * getLocalBounds().toFloat().getAspectRatio (false);
        return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
    }
    
    /** Calculates and returns the View Matrix.
     */
    Matrix3D<float> getViewMatrix() const
    {
        Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -10.0f));
        Matrix3D<float> rotationMatrix = m_draggableOrientation.getRotationMatrix();
        
        return rotationMatrix * viewMatrix;
    }
    
    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
     */
    void createShaders()
    {
        m_vertexShader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 xzPos;\n"
        "layout (location = 1) in float yPos;\n"
        // Uniforms
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projectionMatrix * viewMatrix * vec4(xzPos[0], yPos, xzPos[1], 1.0f);\n"
        "}\n";
   
        
        // Base Shader
        m_fragmentShader =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4 (1.0f, 0.0f, 2.0f, 1.0f);\n"
        "}\n";
        

        ScopedPointer<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (m_openGLContext));
        String statusText;
        
        if (newShader->addVertexShader ((m_vertexShader))
            && newShader->addFragmentShader ((m_fragmentShader))
            && newShader->link())
        {
            m_uniforms = nullptr;
            
            m_shader = newShader;
            m_shader->use();
            
            m_uniforms   = new Uniforms (m_openGLContext, *m_shader);
            
            statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
        }
        else
        {
            statusText = newShader->getLastError();
        }
        
        //m_statusLabel.setText (statusText, dontSendNotification);
    }
    
    //==============================================================================
    // This class manages the uniform values that the shaders use.
    struct Uniforms
    {
        Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
        {
            projectionMatrix = createUniform (openGLContext, shaderProgram, "projectionMatrix");
            viewMatrix       = createUniform (openGLContext, shaderProgram, "viewMatrix");
        }
        
        ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix;
        //ScopedPointer<OpenGLShaderProgram::Uniform> lightPosition;
        
    private:
        static OpenGLShaderProgram::Uniform* createUniform (OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shaderProgram,
                                                            const char* uniformName)
        {
            if (openGLContext.extensions.glGetUniformLocation (shaderProgram.getProgramID(), uniformName) < 0)
                return nullptr;
            
            return new OpenGLShaderProgram::Uniform (shaderProgram, uniformName);
        }
    };

    // Visualizer Variables
    GLfloat m_xFreqWidth;
    GLfloat m_yAmpHeight;
    GLfloat m_zTimeDepth;
    int m_xFreqResolution;
    int m_zTimeResolution;
    
    int m_numVertices;
    GLfloat * m_xzVertices;
    GLfloat * m_yVertices;
    
    
    // OpenGL Variables
    OpenGLContext m_openGLContext;
    GLuint m_xzVBO;
    GLuint m_yVBO;
    GLuint m_VAO;/*, EBO;*/
    
    ScopedPointer<OpenGLShaderProgram> m_shader;
    ScopedPointer<Uniforms> m_uniforms;
    
    const char* m_vertexShader;
    const char* m_fragmentShader;
    
    
    // GUI Interaction
    Draggable3DOrientation m_draggableOrientation;
    
    // Audio Structures
    RingBuffer<GLfloat> * m_ringBuffer;
    AudioBuffer<GLfloat> m_readBuffer;    // Stores data read from ring buffer
    dsp::FFT m_forwardFFT;
    GLfloat * m_fftData;
    
    // This is so that we can initialize fowardFFT in the constructor with the order
    enum
    {
        fftOrder = 10,
        fftSize  = 1 << fftOrder // set 10th bit to one
    };
    
    // Overlay GUI
    Label m_statusLabel;
    
    /** DEV NOTE
        If I wanted to optionally have an interchangeable shader system,
        this would be fairly easy to add. Chack JUCE Demo -> OpenGLDemo.cpp for
        an implementation example of this. For now, we'll just allow these
        shader files to be static instead of interchangeable and dynamic.
        String newVertexShader, newFragmentShader;
     */
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectrum)
};
