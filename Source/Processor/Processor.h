/*
  ==============================================================================

    MainProcessor.h
    Created: 1 Nov 2019 11:26:44am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ProcessorLevelData.h"
#include "ProcessorSpectrumData.h"

//==============================================================================
/*
*/
class Processor :   public AudioProcessor,
                    public Timer
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
    void AddListener(Listener *listener);
    void RemoveListener(Listener *listener);

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
    void timerCallback() override;

private:
    void BroadcastData(AbstractProcessorData *data);
    
    /*dbg*/
    ProcessorLevelData PrepareNextLevelData();
    ProcessorSpectrumData PrepareNextSpectrumData();
    
    float m_dummyCalcBase;
    ProcessorLevelData m_dummyLD;
    /*dbg*/
    
    String              m_Name;
    Array<Listener*>    m_callbackListeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
