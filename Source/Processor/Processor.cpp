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
	m_sampleRate = sampleRate;
	m_samplesPerCentiSecond = sampleRate * 0.01f;
	m_bufferSize = maximumExpectedSamplesPerBlock;
	m_missingSamplesForCentiSecond = int(m_samplesPerCentiSecond + 0.5f);
	m_centiSecondBuffer.setSize(2, m_missingSamplesForCentiSecond, false, true, false);
}

void Processor::releaseResources()
{
	m_sampleRate = 0;
	m_samplesPerCentiSecond = 0;
	m_bufferSize = 0;
	m_centiSecondBuffer.clear();
	m_missingSamplesForCentiSecond = 0;
}

void Processor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ignoreUnused(midiMessages);

	int numChannels = buffer.getNumChannels();

	if (numChannels != m_centiSecondBuffer.getNumChannels())
		m_centiSecondBuffer.setSize(numChannels, m_samplesPerCentiSecond, false, true, true);

	int availableSamples = buffer.getNumSamples();

	int readPos = 0;
	int writePos = m_samplesPerCentiSecond - m_missingSamplesForCentiSecond;
	while (availableSamples >= m_missingSamplesForCentiSecond)
	{
		for (int i = 0; i < numChannels; ++i)
		{
			m_centiSecondBuffer.copyFrom(i, writePos, buffer.getReadPointer(i) + readPos, m_missingSamplesForCentiSecond);

			m_level.SetLevel(i + 1, ProcessorLevelData::LevelVal(m_centiSecondBuffer.getMagnitude(i, 0, m_samplesPerCentiSecond), m_centiSecondBuffer.getRMSLevel(i, 0, m_samplesPerCentiSecond)));
		}
		BroadcastData(&m_level);

		BroadcastData(&m_centiSecondBuffer);

		readPos += m_missingSamplesForCentiSecond;
		availableSamples -= m_missingSamplesForCentiSecond;

		m_missingSamplesForCentiSecond = m_samplesPerCentiSecond;

		writePos = m_samplesPerCentiSecond - m_missingSamplesForCentiSecond;

		if (availableSamples <= 0)
			break;
	}
		
	if (availableSamples > 0)
	{
		for (int i = 0; i < numChannels; ++i)
		{
			m_centiSecondBuffer.copyFrom(i, writePos, buffer.getReadPointer(i) + readPos, availableSamples);
		}
	}

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
		prepareToPlay(device->getCurrentSampleRate(), device->getCurrentBufferSizeSamples());
	}
}

void Processor::audioDeviceStopped()
{
	releaseResources();
}

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
