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
    /*dbg*/m_dummySignalCalcBase = 0.0f;
	/*dbg*/m_dummyLevelCalcBase = 0.0f;
	/*dbg*/m_dummySpectrumCalcBase = 0.0f;
    
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


ProcessorAudioSignalData Processor::PrepareNextSignalData()
{
	float nextPeak1Signal = sin(m_dummySignalCalcBase);
	float nextRms1Signal = 0.9f * nextPeak1Signal;
	m_dummySignal.SetAudioSignal(1, ProcessorAudioSignalData::SignalVal(nextPeak1Signal, nextRms1Signal));
	m_dummySignalCalcBase += 0.1f;

	float nextPeak2Signal = sin(m_dummySignalCalcBase);
	float nextRms2Signal = 0.9f * nextPeak2Signal;
	m_dummySignal.SetAudioSignal(2, ProcessorAudioSignalData::SignalVal(nextPeak2Signal, nextRms2Signal));
	m_dummySignalCalcBase += 0.1f;

	float nextPeak3Signal = sin(m_dummySignalCalcBase);
	float nextRms3Signal = 0.9f * nextPeak3Signal;
	m_dummySignal.SetAudioSignal(3, ProcessorAudioSignalData::SignalVal(nextPeak3Signal, nextRms3Signal));
	m_dummySignalCalcBase += 0.1f;

	float nextPeak4Signal = sin(m_dummySignalCalcBase);
	float nextRms4Signal = 0.9f * nextPeak4Signal;
	m_dummySignal.SetAudioSignal(4, ProcessorAudioSignalData::SignalVal(nextPeak4Signal, nextRms4Signal));
	m_dummySignalCalcBase += 0.1f;

	float nextPeak5Signal = sin(m_dummySignalCalcBase);
	float nextRms5Signal = 0.9f * nextPeak5Signal;
	m_dummySignal.SetAudioSignal(5, ProcessorAudioSignalData::SignalVal(nextPeak5Signal, nextRms5Signal));
	m_dummySignalCalcBase += 0.1f;

	return m_dummySignal;
}

ProcessorLevelData Processor::PrepareNextLevelData()
{
    float nextPeak1Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
    float nextRms1Level = 0.9f*nextPeak1Level;
    m_dummyLevel.SetLevel(1, ProcessorLevelData::LevelVal(nextPeak1Level, nextRms1Level));
    m_dummyLevelCalcBase += 0.1f;
    
    float nextPeak2Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
    float nextRms2Level = 0.9f*nextPeak2Level;
	m_dummyLevel.SetLevel(2, ProcessorLevelData::LevelVal(nextPeak2Level, nextRms2Level));
	m_dummyLevelCalcBase += 0.1f;
    
    float nextPeak3Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
    float nextRms3Level = 0.9f*nextPeak3Level;
	m_dummyLevel.SetLevel(3, ProcessorLevelData::LevelVal(nextPeak3Level, nextRms3Level));
	m_dummyLevelCalcBase += 0.1f;
    
    float nextPeak4Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
    float nextRms4Level = 0.9f*nextPeak4Level;
	m_dummyLevel.SetLevel(4, ProcessorLevelData::LevelVal(nextPeak4Level, nextRms4Level));
	m_dummyLevelCalcBase += 0.1f;
    
    float nextPeak5Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
    float nextRms5Level = 0.9f*nextPeak5Level;
	m_dummyLevel.SetLevel(5, ProcessorLevelData::LevelVal(nextPeak5Level, nextRms5Level));
	m_dummyLevelCalcBase += 0.1f;
    
    return m_dummyLevel;
}

ProcessorSpectrumData Processor::PrepareNextSpectrumData()
{
    return ProcessorSpectrumData();
}

void Processor::timerCallback()
{
	ProcessorAudioSignalData signalData = PrepareNextSignalData();
	BroadcastData(&signalData);

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
