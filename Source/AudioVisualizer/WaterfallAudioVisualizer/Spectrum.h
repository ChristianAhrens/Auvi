#pragma once

#include <JuceHeader.h>

#include "RingBuffer.h"

/** Frequency Spectrum visualizer. Uses basic shaders, and calculates all points
    on the CPU as opposed to the OScilloscope3D which calculates points on the
    GPU.
 */

class Spectrum :    public OpenGLAppComponent
{
    
public:
    Spectrum (RingBuffer<GLfloat> * ringBuffer);
    ~Spectrum();
    
    void setRingBuffer(RingBuffer<GLfloat> * ringBuffer);
    
    //==========================================================================
    // Oscilloscope Control Functions
    void start();
    void stop();
    void setChannel(int channel);
    
    //==========================================================================
    // OpenGLAppComponent OpenGL wrapper methods
    void initialise() override; // called from call to private OpenGLAppComponent::newOpenGLContextCreated
    void shutdown() override; // called from call to private OpenGLAppComponent::openGLContextClosing
    void render() override; // called from call to private OpenGLAppComponent::renderOpenGL
    
    //==========================================================================
    // JUCE Callbacks
    void paint (Graphics& g) override;
    void resized () override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    
private:
    //==========================================================================
    // Mesh Functions
    
    // Initialize the XZ values of vertices
    void initializeXZVertices();
    
    // Initialize the Y valies of vertices
    void initializeYVertices();
    
    
    //==========================================================================
    // OpenGL Functions
    
    /** Calculates and returns the Projection Matrix.
     */
    Matrix3D<float> getProjectionMatrix() const;
    
    /** Calculates and returns the View Matrix.
     */
    Matrix3D<float> getViewMatrix() const;

    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
     */
    void createShaders();
    void createVertexShader();
    void createFragmentShader();
    
    //==============================================================================
    // This class manages the uniform values that the shaders use.
    struct Uniforms
    {
        Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
        {
            projectionMatrix = createUniform (openGLContext, shaderProgram, "projectionMatrix");
            viewMatrix       = createUniform (openGLContext, shaderProgram, "viewMatrix");
        }
        
        std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix;
        //std::unique_ptr<OpenGLShaderProgram::Uniform> lightPosition;
        
    private:
        static std::unique_ptr<OpenGLShaderProgram::Uniform> createUniform (OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shaderProgram,
                                                            const char* uniformName)
        {
            if (openGLContext.extensions.glGetUniformLocation (shaderProgram.getProgramID(), uniformName) < 0)
                return nullptr;
            
            return std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgram, uniformName);
        }
    };

    // Visualizer Variables
    int m_channel{ 0 };

    GLfloat m_xFreqWidth;
    GLfloat m_yAmpHeight;
    GLfloat m_zTimeDepth;
    int m_xFreqResolution;
    int m_zTimeResolution;
    
    int m_numVertices;
    GLfloat * m_xzVertices;
    GLfloat * m_yVertices;
    
    
    // OpenGL Variables
    GLuint m_xzVBO;
    GLuint m_yVBO;
    GLuint m_VAO;/*, EBO;*/
    
    std::unique_ptr<OpenGLShaderProgram> m_shader;
    std::unique_ptr<Uniforms> m_uniforms;
    
    String m_vertexShader;
    String m_fragmentShader;
    
    
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
