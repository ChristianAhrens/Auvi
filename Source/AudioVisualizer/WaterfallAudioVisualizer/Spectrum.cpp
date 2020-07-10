#include "Spectrum.h"

#define RING_BUFFER_READ_SIZE 256


Spectrum::Spectrum (RingBuffer<GLfloat> * ringBuffer)
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

Spectrum::~Spectrum()
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

void Spectrum::start()
{
	m_openGLContext.setContinuousRepainting (true);
}

void Spectrum::stop()
{
	m_openGLContext.setContinuousRepainting (false);
}

void Spectrum::setChannel(int channel)
{
	if (channel > -1)
		m_channel = channel;
	else
		jassertfalse;
}


//==========================================================================
// OpenGL Callbacks

/** Called before rendering OpenGL, after an OpenGLContext has been associated
	with this OpenGLRenderer (this component is a OpenGLRenderer).
	Sets up GL objects that are needed for rendering.
 */
void Spectrum::newOpenGLContextCreated()
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
	
#if JUCE_OPENGL3
	m_openGLContext.extensions.glGenVertexArrays(1, &m_VAO);
	m_openGLContext.extensions.glBindVertexArray(m_VAO);
#endif
	m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_xzVBO);
	m_openGLContext.extensions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	m_openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, m_yVBO);
	m_openGLContext.extensions.glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), NULL);
	
	m_openGLContext.extensions.glEnableVertexAttribArray (0);
	m_openGLContext.extensions.glEnableVertexAttribArray (1);

#if JUCE_OPENGL3
#if JUCE_OPENGL_ES
		glEnable(GL_POINT_SPRITE);
#else
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
#endif
	
	// Setup Shaders
	createShaders();
}

/** Called when done rendering OpenGL, as an OpenGLContext object is closing.
	Frees any GL objects created during rendering.
 */
void Spectrum::openGLContextClosing()
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
void Spectrum::renderOpenGL()
{
	jassert (OpenGLHelpers::isContextActive());
	if (!m_shader)
		return;
	
	// Setup Viewport
	const float renderingScale = (float)m_openGLContext.getRenderingScale();
	glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
	
	// Set background Color
	OpenGLHelpers::clear (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).darker());
	
	// Enable Alpha Blending
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Use Shader Program that's been defined
	m_shader->use();
	
	// Copy data from ring buffer into FFT
	m_ringBuffer->readSamples (m_readBuffer, RING_BUFFER_READ_SIZE);
	FloatVectorOperations::clear (m_fftData, RING_BUFFER_READ_SIZE);
	
	///** Future Feature:
	//	Instead of summing channels below, keep the channels seperate and
	//	lay out the spectrum so you can see the left and right channels
	//	individually on either half of the spectrum.
	// */
	//// Sum channels together
	//for (int i = 0; i < 2; ++i)
	//{
	//	FloatVectorOperations::add (m_fftData, m_readBuffer.getReadPointer(i, 0), RING_BUFFER_READ_SIZE);
	//}

	if(m_channel < m_readBuffer.getNumChannels())
		FloatVectorOperations::add(m_fftData, m_readBuffer.getReadPointer(m_channel, 0), RING_BUFFER_READ_SIZE);
	
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
	m_openGLContext.extensions.glBindVertexArray(m_VAO);
#endif
	glDrawArrays (GL_POINTS, 0, m_numVertices);
	
	
	// Zero Out FFT for next use
	zeromem (m_fftData, sizeof (GLfloat) * 2 * fftSize);
	
	// Reset the element buffers so child Components draw correctly
//        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
//        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
//        openGLContext.extensions.glBindVertexArray(0);
}

void Spectrum::paint (Graphics& g) {}

void Spectrum::resized ()
{
	m_draggableOrientation.setViewport (getLocalBounds());
	m_statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
}

void Spectrum::mouseDown (const MouseEvent& e)
{
	m_draggableOrientation.mouseDown (e.getPosition());
}

void Spectrum::mouseDrag (const MouseEvent& e)
{
	m_draggableOrientation.mouseDrag (e.getPosition());
}

void Spectrum::initializeXZVertices()
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

void Spectrum::initializeYVertices()
{
	// Set all Y values to 0.0
	m_yVertices = new GLfloat [m_numVertices];
	memset(m_yVertices, 0.0f, sizeof(GLfloat) * m_xFreqResolution * m_zTimeResolution);
}

/** Calculates and returns the Projection Matrix.
 */
Matrix3D<float> Spectrum::getProjectionMatrix() const
{
	float w = 1.0f / (0.5f + 0.1f);
	float h = w * getLocalBounds().toFloat().getAspectRatio (false);
	return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
}

/** Calculates and returns the View Matrix.
 */
Matrix3D<float> Spectrum::getViewMatrix() const
{
	Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -10.0f));
	Matrix3D<float> rotationMatrix = m_draggableOrientation.getRotationMatrix();
	
	return rotationMatrix * viewMatrix;
}

const char* Spectrum::createVertexShader()
{
	auto GLVersion = OpenGLShaderProgram::getLanguageVersion();

	return 
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
		"    gl_PointSize = 5.0f;\n"
		"}\n";
}

const char* Spectrum::createFragmentShader()
{
	auto GLVersion = OpenGLShaderProgram::getLanguageVersion();

	juce::Colour color = Colours::forestgreen.darker();
	// Base Shader
	auto fragmentShaderString = String(
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"    color = vec4 ("
		+ String(color.getFloatRed()) + ", "
		+ String(color.getFloatGreen()) + ", "
		+ String(color.getFloatBlue()) + ", "
		+ String(color.getFloatAlpha()) + ");\n"
		"}\n");

	return fragmentShaderString.toUTF8();
}

/** Loads the OpenGL Shaders and sets up the whole ShaderProgram
 */
void Spectrum::createShaders()
{
	m_vertexShader = createVertexShader();
	m_fragmentShader = createFragmentShader();
	
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
		
#ifdef DEBUG
		statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
#endif
	}
	else
	{
		statusText = newShader->getLastError();
	}
	
	m_statusLabel.setText (statusText, dontSendNotification);
}
