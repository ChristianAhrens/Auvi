/*
  ==============================================================================

    Processor.cpp
    Created: 1 Nov 2019 11:26:44am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "Processor.h"
#include "ProcessorLevelData.h"
#include "ProcessorSpectrumData.h"

//==============================================================================
Processor::Processor()
    : AudioProcessor()
{
    /*dbg*/m_dummyCalcBase = 0;
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    startTimer(100);
}

Processor::~Processor()
{
}

void Processor::AddListener(Listener *listener)
{
    m_callbackListeners.add(listener);
}

void Processor::RemoveListener(Listener *listener)
{
    m_callbackListeners.remove(m_callbackListeners.indexOf(listener));
}

//==============================================================================
const String Processor::getName() const
{
    return m_Name;
}

void Processor::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void Processor::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void Processor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    /*dbg*/ignoreUnused(buffer);
    /*dbg*/ignoreUnused(midiMessages);
}

double Processor::getTailLengthSeconds() const
{
    /*dbg*/return 0.0;
}

bool Processor::acceptsMidi() const
{
    return false;
}

bool Processor::producesMidi() const
{
    return false;
}

AudioProcessorEditor* Processor::createEditor()
{
    /*dbg*/return nullptr;
}

bool Processor::hasEditor() const
{
    /*dbg*/return false;
}

int Processor::getNumPrograms()
{
    /*dbg*/return 0;
}

int Processor::getCurrentProgram()
{
    /*dbg*/return 0;
}

void Processor::setCurrentProgram (int index)
{
    /*dbg*/ignoreUnused(index);
}

const String Processor::getProgramName (int index)
{
    /*dbg*/ignoreUnused(index);
    /*dbg*/return String();
}

void Processor::changeProgramName (int index, const String& newName)
{
    /*dbg*/ignoreUnused(index);
    /*dbg*/ignoreUnused(newName);
}

void Processor::getStateInformation (juce::MemoryBlock& destData)
{
    /*dbg*/ignoreUnused(destData);
}

void Processor::setStateInformation (const void* data, int sizeInBytes)
{
    /*dbg*/ignoreUnused(data);
    /*dbg*/ignoreUnused(sizeInBytes);
}

ProcessorLevelData Processor::PrepareNextLevelData()
{
    float nextRms1Level = 0.5f*(1+sin(m_dummyCalcBase));
    float nextPeak1Level = nextRms1Level + 0.1f;
    m_dummyLD.SetLevel(1, ProcessorLevelData::LevelVal(nextRms1Level, nextPeak1Level));
    m_dummyCalcBase += 0.1f;
    
    float nextRms2Level = 0.5f*(1+sin(m_dummyCalcBase));
    float nextPeak2Level = nextRms2Level + 0.1f;
    m_dummyLD.SetLevel(2, ProcessorLevelData::LevelVal(nextRms2Level, nextPeak2Level));
    m_dummyCalcBase += 0.1f;
    
    float nextRms3Level = 0.5f*(1+sin(m_dummyCalcBase));
    float nextPeak3Level = nextRms3Level + 0.1f;
    m_dummyLD.SetLevel(3, ProcessorLevelData::LevelVal(nextRms3Level, nextPeak3Level));
    m_dummyCalcBase += 0.1f;
    
    float nextRms4Level = 0.5f*(1+sin(m_dummyCalcBase));
    float nextPeak4Level = nextRms4Level + 0.1f;
    m_dummyLD.SetLevel(4, ProcessorLevelData::LevelVal(nextRms4Level, nextPeak4Level));
    m_dummyCalcBase += 0.1f;
    
    float nextRms5Level = 0.5f*(1+sin(m_dummyCalcBase));
    float nextPeak5Level = nextRms5Level + 0.1f;
    m_dummyLD.SetLevel(5, ProcessorLevelData::LevelVal(nextRms5Level, nextPeak5Level));
    m_dummyCalcBase += 0.1f;
    
    return m_dummyLD;
}

ProcessorSpectrumData Processor::PrepareNextSpectrumData()
{
    return ProcessorSpectrumData();
}

void Processor::timerCallback()
{
    ProcessorLevelData levelData = PrepareNextLevelData();
    BroadcastData(&levelData);
    
    ProcessorSpectrumData spectrumData = PrepareNextSpectrumData();
    BroadcastData(&spectrumData);
}

void Processor::BroadcastData(AbstractProcessorData *data)
{
    for(Listener *l : m_callbackListeners)
        l->processingDataChanged(data);
}
