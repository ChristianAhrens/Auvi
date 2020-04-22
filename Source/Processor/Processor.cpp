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
}

Processor::~Processor()
{
}

void Processor::setPauseProcessing(bool pause)
{
	m_pauseProcessing = pause;
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
				m_level.SetLevel(i + 1, ProcessorLevelData::LevelVal(m_centiSecondBuffer.getMagnitude(i, 0, m_samplesPerCentiSecond), m_centiSecondBuffer.getRMSLevel(i, 0, m_samplesPerCentiSecond)));

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
						ProcessorSpectrumData::SpectrumBands spectrumBands;

						spectrumBands.mindB = -100.0f;
						spectrumBands.maxdB = 0.0f;

						spectrumBands.minFreq = m_sampleRate / ProcessorSpectrumData::SpectrumBands::count;
						spectrumBands.maxFreq = m_sampleRate / 2;
						spectrumBands.freqRes = (spectrumBands.maxFreq - spectrumBands.minFreq) / ProcessorSpectrumData::SpectrumBands::count;

						for (auto j = 0; j < ProcessorSpectrumData::SpectrumBands::count; ++j)
						{
							auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (j / static_cast<float>(ProcessorSpectrumData::SpectrumBands::count) * 0.2f)));
							auto fftDataIndex = jlimit(0, fftSize / 2, static_cast<int>(skewedProportionX * fftSize / 2));
							auto level = jmap(jlimit(spectrumBands.mindB, spectrumBands.maxdB, Decibels::gainToDecibels(m_FFTdata[fftDataIndex])),
								spectrumBands.mindB, spectrumBands.maxdB, 0.0f, 1.0f);

							spectrumBands.bands[j] = level;
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
