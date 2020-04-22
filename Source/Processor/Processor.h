/*
  ==============================================================================

    MainProcessor.h
    Created: 1 Nov 2019 11:26:44am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ProcessorAudioSignalData.h"
#include "ProcessorLevelData.h"
#include "ProcessorSpectrumData.h"

//==============================================================================
/*
*/
class AudioBufferMessage : public Message
{
public:
    AudioBufferMessage(AudioBuffer<float>& buffer);
    ~AudioBufferMessage();

    const AudioBuffer<float>& getAudioBuffer() const;

private:
    AudioBuffer<float> m_buffer;
};

//==============================================================================
/*
*/
class Processor :   public AudioProcessor,
					public AudioIODeviceCallback,
                    public MessageListener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        
        virtual void processingDataChanged(AbstractProcessorData *data) = 0;
    };
    
public:
    Processor();
    ~Processor();

    //==============================================================================
    void setPauseProcessing(bool pause);
    
    //==============================================================================
    void addListener(Listener *listener);
    void removeListener(Listener *listener);

    //==============================================================================
    const String getName() const override;
    void prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
    
    double getTailLengthSeconds() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================
	void audioDeviceIOCallback(const float** inputChannelData,
		int numInputChannels,
		float** outputChannelData,
		int numOutputChannels,
		int numSamples) override;
	void audioDeviceAboutToStart(AudioIODevice* device) override;
	void audioDeviceStopped() override;

    //==============================================================================
    void handleMessage(const Message& message) override;

private:
    void BroadcastData(AbstractProcessorData *data);

    ProcessorAudioSignalData    m_centiSecondBuffer;
	ProcessorLevelData          m_level;
    ProcessorSpectrumData       m_spectrum;
    
    String              m_Name;
    Array<Listener*>    m_callbackListeners;

	//==============================================================================
	CriticalSection m_readLock;
	double m_sampleRate = 0;
    double m_samplesPerCentiSecond = 0;
	int m_bufferSize = 0;
	
	float* m_processorChannels[128];
    AudioBuffer<float> m_buffer;
    int m_missingSamplesForCentiSecond;

	const float* inputChans[128]; // this is only a member to enshure it is not recreated on every function call

    //==============================================================================
    enum
    {
        fftOrder = 12,
        fftSize = 1 << fftOrder
    };
    dsp::FFT                        m_fwdFFT;
    dsp::WindowingFunction<float>   m_windowF;
    float                           m_FFTdata[2*fftSize];
    int                             m_FFTdataPos;

    bool                            m_pauseProcessing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
