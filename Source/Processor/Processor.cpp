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
AudioBufferMessage::AudioBufferMessage(AudioBuffer<float>& buffer)
{
	m_buffer = buffer;
}

AudioBufferMessage::~AudioBufferMessage()
{

}

const AudioBuffer<float>& AudioBufferMessage::getAudioBuffer() const
{
	return m_buffer;
}

//==============================================================================
Processor::Processor() :
    AudioProcessor(), 
	m_fwdFFT(fftOrder), 
	m_windowF(fftSize, dsp::WindowingFunction<float>::hann)
{
	m_FFTdataPos = 0;
	zeromem(m_FFTdata, sizeof(m_FFTdata));
	m_pauseProcessing = false;
	
	setHoldTime(500);
}

Processor::~Processor()
{
}

void Processor::setPauseProcessing(bool pause)
{
	m_pauseProcessing = pause;
}

void Processor::setHoldTime(int holdTimeMs)
{
	m_holdTimeMs = holdTimeMs;

	startTimer(m_holdTimeMs);
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
    m_samplesPerCentiSecond = std::round(sampleRate * 0.01f);
    m_bufferSize = maximumExpectedSamplesPerBlock;
    m_missingSamplesForCentiSecond = static_cast<int>(m_samplesPerCentiSecond + 0.5f);
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

void Processor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ignoreUnused(midiMessages);

	postMessage(new AudioBufferMessage(buffer));
}

void Processor::handleMessage(const Message& message)
{
	if (m_pauseProcessing)
		return;

	if (auto m = dynamic_cast<const AudioBufferMessage*> (&message))
	{
		auto buffer = m->getAudioBuffer();

		int numChannels = buffer.getNumChannels();

		if (numChannels != m_centiSecondBuffer.getNumChannels())
			m_centiSecondBuffer.setSize(numChannels, m_samplesPerCentiSecond, false, true, true);
		if (m_sampleRate != m_centiSecondBuffer.GetSampleRate())
			m_centiSecondBuffer.SetSampleRate(m_sampleRate);

		int availableSamples = buffer.getNumSamples();

		int readPos = 0;
		int writePos = m_samplesPerCentiSecond - m_missingSamplesForCentiSecond;
		while (availableSamples >= m_missingSamplesForCentiSecond)
		{
			for (int i = 0; i < numChannels; ++i)
			{
				// generate signal buffer data
				m_centiSecondBuffer.copyFrom(i, writePos, buffer.getReadPointer(i) + readPos, m_missingSamplesForCentiSecond);

				// generate level data
				auto peak = m_centiSecondBuffer.getMagnitude(i, 0, m_samplesPerCentiSecond);
				auto rms = m_centiSecondBuffer.getRMSLevel(i, 0, m_samplesPerCentiSecond);
				auto hold = std::max(peak, m_level.GetLevel(i + 1).hold);
				m_level.SetLevel(i + 1, ProcessorLevelData::LevelVal(peak, rms, hold));

				// generate spectrum data
				{
					int unprocessedSamples = 0;
					if (m_FFTdataPos < fftSize)
					{
						int missingSamples = fftSize - m_FFTdataPos;
						if (missingSamples < m_samplesPerCentiSecond)
						{
							memcpy(m_FFTdata, m_centiSecondBuffer.getReadPointer(i), missingSamples);
							m_FFTdataPos += missingSamples;
							unprocessedSamples = m_samplesPerCentiSecond - missingSamples;
						}
						else
						{
							memcpy(m_FFTdata, m_centiSecondBuffer.getReadPointer(i), m_samplesPerCentiSecond);
							m_FFTdataPos += m_samplesPerCentiSecond;
						}
					}

					if (m_FFTdataPos >= fftSize)
					{
						m_windowF.multiplyWithWindowingTable(m_FFTdata, fftSize);
						m_fwdFFT.performFrequencyOnlyForwardTransform(m_FFTdata);
						ProcessorSpectrumData::SpectrumBands spectrumBands = m_spectrum.GetSpectrum(i);

						spectrumBands.mindB = -100.0f;
						spectrumBands.maxdB = 0.0f;

						spectrumBands.minFreq = m_sampleRate / ProcessorSpectrumData::SpectrumBands::count;
						spectrumBands.maxFreq = m_sampleRate / 2;
						spectrumBands.freqRes = (spectrumBands.maxFreq - spectrumBands.minFreq) / ProcessorSpectrumData::SpectrumBands::count;

						int spectrumStepWidth = 0.5f * (fftSize / ProcessorSpectrumData::SpectrumBands::count);
						int spectrumPos = 0;
						for (int j = 0; j < ProcessorSpectrumData::SpectrumBands::count && spectrumPos < fftSize; ++j)
						{
							float spectrumVal = 0;
							
							for (int k = 0; k < spectrumStepWidth; ++k, ++spectrumPos)
								spectrumVal += m_FFTdata[spectrumPos];
							spectrumVal = spectrumVal / spectrumStepWidth;

							auto leveldB = jlimit(spectrumBands.mindB, spectrumBands.maxdB, Decibels::gainToDecibels(spectrumVal));
							auto level = jmap(leveldB, spectrumBands.mindB, spectrumBands.maxdB, 0.0f, 1.0f);

							spectrumBands.bandsPeak[j] = level;
							spectrumBands.bandsHold[j] = std::max(level, spectrumBands.bandsHold[j]);
						}

						m_spectrum.SetSpectrum(i, spectrumBands);

						zeromem(m_FFTdata, sizeof(m_FFTdata));
						m_FFTdataPos = 0;
					}

					if (unprocessedSamples != 0)
					{
						memcpy(m_FFTdata, m_centiSecondBuffer.getReadPointer(i, m_samplesPerCentiSecond - unprocessedSamples), unprocessedSamples);
						m_FFTdataPos += unprocessedSamples;
					}
				}
			}

			BroadcastData(&m_level);
			BroadcastData(&m_centiSecondBuffer);
			BroadcastData(&m_spectrum);

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

void Processor::BroadcastData(AbstractProcessorData *data)
{
    for(Listener *l : m_callbackListeners)
        l->processingDataChanged(data);
}

void Processor::timerCallback()
{
	FlushHold();
}

void Processor::FlushHold()
{
	// clear level hold values
	auto channelCount = m_level.GetChannelCount();
	for (int i = 0; i < channelCount; ++i)
	{
		m_level.SetLevel(i + 1, ProcessorLevelData::LevelVal(0.0f, 0.0f, 0.0f));
	}

	// clear spectrum hold values	auto channelCount = m_level.GetChannelCount();
	channelCount = m_spectrum.GetChannelCount();
	for (int i = 0; i < channelCount; ++i)
	{
		ProcessorSpectrumData::SpectrumBands spectrumBands = m_spectrum.GetSpectrum(i);
		for (int j = 0; j < ProcessorSpectrumData::SpectrumBands::count; ++j)
		{
			spectrumBands.bandsPeak[j] = 0.0f;
			spectrumBands.bandsHold[j] = 0.0f;
		}

		m_spectrum.SetSpectrum(i, spectrumBands);
	}
}