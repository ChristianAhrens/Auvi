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
    ///*dbg*/m_dummySignalCalcBase = 0.0f;
	///*dbg*/m_dummyLevelCalcBase = 0.0f;
	/*dbg*/m_dummySpectrumCalcBase = 0.0f;
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    startTimer(100);
}

Processor::~Processor()
{
}

void Processor::addListener(Listener *listener)
{
    m_callbackListeners.add(listener);
}

void Processor::removeListener(Listener *listener)
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
	/*dbg*/jassertfalse; // aktuell nicht erwartet
	m_sampleRate = sampleRate;
	m_bufferSize = maximumExpectedSamplesPerBlock;
}

void Processor::releaseResources()
{
	/*dbg*/jassertfalse; // aktuell nicht erwartet
	m_sampleRate = 0;
	m_bufferSize = 0;
}

void Processor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ignoreUnused(midiMessages);

	int numChannels = buffer.getNumChannels();
	int numSamples = buffer.getNumSamples();

	float rms = 0.0f;
	float peak = 0.0f;

	for (int i = 0; i < numChannels; ++i)
	{
		const float *readBuffer = buffer.getReadPointer(i);

		for (int j = 0; j < numSamples; ++j)
		{
			// das ist quatsch
			peak = std::max(std::abs(readBuffer[j]), peak);
			rms += std::abs(readBuffer[j]);
		}
		rms = rms / numSamples;

		m_signal.SetAudioSignal(i + 1, ProcessorAudioSignalData::SignalVal(peak, rms));
		m_level.SetLevel(i + 1, ProcessorLevelData::LevelVal(peak, rms));
	}

	BroadcastData(&m_signal);
	BroadcastData(&m_level);
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

void Processor::audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, 
	float** outputChannelData, int numOutputChannels, int numSamples)
{
	ignoreUnused(outputChannelData);
	ignoreUnused(numOutputChannels);

	// these should have been prepared by audioDeviceAboutToStart()...
	jassert(m_sampleRate > 0 && m_bufferSize > 0);

	const ScopedLock sl(m_readLock);

	int numActiveChans = 0;
	int numInputs = 0;

	// messy stuff needed to compact the channels down into an array
	// of non-zero pointers..
	for (int i = 0; i < numInputChannels; ++i)
	{
		if (inputChannelData[i] != nullptr)
		{
			inputChans[numInputs++] = inputChannelData[i];
			if (numInputs >= numElementsInArray(inputChans))
				break;
		}
	}

	if (numInputs > m_buffer.getNumChannels())
	{
		numActiveChans = m_buffer.getNumChannels();

		// if there aren't enough output channels for the number of
		// inputs, we need to create some temporary extra ones (can't
		// use the input data in case it gets written to)
		m_buffer.setSize(numInputs, numSamples,
			false, false, true);

		for (int i = numActiveChans; i < numInputs; ++i)
		{
			m_processorChannels[numActiveChans] = m_buffer.getWritePointer(i);
			memcpy(m_processorChannels[numActiveChans], inputChans[i], (size_t)numSamples * sizeof(float));
			++numActiveChans;
		}
	}
	else
	{
		for (int i = 0; i < numInputs; ++i)
		{
			memcpy(m_processorChannels[numActiveChans], inputChans[i], (size_t)numSamples * sizeof(float));
			++numActiveChans;
		}
	}

    AudioBuffer<float> newAudioBuffer(m_processorChannels, numActiveChans, numSamples);
    MidiBuffer newMidiBuffer;
    
	processBlock(newAudioBuffer, newMidiBuffer);
}

void Processor::audioDeviceAboutToStart(AudioIODevice* device)
{
	if(device)
	{
		m_sampleRate = device->getCurrentSampleRate();
		m_bufferSize = device->getCurrentBufferSizeSamples();
	}
}

void Processor::audioDeviceStopped()
{
	m_sampleRate = 0;
	m_bufferSize = 0;
}

//ProcessorAudioSignalData Processor::PrepareNextSignalData()
//{
//	float nextPeak1Signal = sin(m_dummySignalCalcBase);
//	float nextRms1Signal = 0.9f * nextPeak1Signal;
//	m_dummySignal.SetAudioSignal(1, ProcessorAudioSignalData::SignalVal(nextPeak1Signal, nextRms1Signal));
//	m_dummySignalCalcBase += 0.1f;
//
//	float nextPeak2Signal = sin(m_dummySignalCalcBase);
//	float nextRms2Signal = 0.9f * nextPeak2Signal;
//	m_dummySignal.SetAudioSignal(2, ProcessorAudioSignalData::SignalVal(nextPeak2Signal, nextRms2Signal));
//	m_dummySignalCalcBase += 0.1f;
//
//	float nextPeak3Signal = sin(m_dummySignalCalcBase);
//	float nextRms3Signal = 0.9f * nextPeak3Signal;
//	m_dummySignal.SetAudioSignal(3, ProcessorAudioSignalData::SignalVal(nextPeak3Signal, nextRms3Signal));
//	m_dummySignalCalcBase += 0.1f;
//
//	float nextPeak4Signal = sin(m_dummySignalCalcBase);
//	float nextRms4Signal = 0.9f * nextPeak4Signal;
//	m_dummySignal.SetAudioSignal(4, ProcessorAudioSignalData::SignalVal(nextPeak4Signal, nextRms4Signal));
//	m_dummySignalCalcBase += 0.1f;
//
//	float nextPeak5Signal = sin(m_dummySignalCalcBase);
//	float nextRms5Signal = 0.9f * nextPeak5Signal;
//	m_dummySignal.SetAudioSignal(5, ProcessorAudioSignalData::SignalVal(nextPeak5Signal, nextRms5Signal));
//	m_dummySignalCalcBase += 0.1f;
//
//	return m_dummySignal;
//}
//
//ProcessorLevelData Processor::PrepareNextLevelData()
//{
//    float nextPeak1Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
//    float nextRms1Level = 0.9f*nextPeak1Level;
//    m_dummyLevel.SetLevel(1, ProcessorLevelData::LevelVal(nextPeak1Level, nextRms1Level));
//    m_dummyLevelCalcBase += 0.1f;
//    
//    float nextPeak2Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
//    float nextRms2Level = 0.9f*nextPeak2Level;
//	m_dummyLevel.SetLevel(2, ProcessorLevelData::LevelVal(nextPeak2Level, nextRms2Level));
//	m_dummyLevelCalcBase += 0.1f;
//    
//    float nextPeak3Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
//    float nextRms3Level = 0.9f*nextPeak3Level;
//	m_dummyLevel.SetLevel(3, ProcessorLevelData::LevelVal(nextPeak3Level, nextRms3Level));
//	m_dummyLevelCalcBase += 0.1f;
//    
//    float nextPeak4Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
//    float nextRms4Level = 0.9f*nextPeak4Level;
//	m_dummyLevel.SetLevel(4, ProcessorLevelData::LevelVal(nextPeak4Level, nextRms4Level));
//	m_dummyLevelCalcBase += 0.1f;
//    
//    float nextPeak5Level = 0.5f*(1+sin(m_dummyLevelCalcBase));
//    float nextRms5Level = 0.9f*nextPeak5Level;
//	m_dummyLevel.SetLevel(5, ProcessorLevelData::LevelVal(nextPeak5Level, nextRms5Level));
//	m_dummyLevelCalcBase += 0.1f;
//    
//    return m_dummyLevel;
//}

ProcessorSpectrumData Processor::PrepareNextSpectrumData()
{
    return ProcessorSpectrumData();
}

void Processor::timerCallback()
{
	//ProcessorAudioSignalData signalData = PrepareNextSignalData();
	//BroadcastData(&signalData);

    //ProcessorLevelData levelData = PrepareNextLevelData();
    //BroadcastData(&levelData);
    
    ProcessorSpectrumData spectrumData = PrepareNextSpectrumData();
    BroadcastData(&spectrumData);
}

void Processor::BroadcastData(AbstractProcessorData *data)
{
	MessageManagerLock lock;

    for(Listener *l : m_callbackListeners)
        l->processingDataChanged(data);
}
