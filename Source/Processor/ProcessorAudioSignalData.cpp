/*
  ==============================================================================

    ProcessorAudioSignalData.cpp
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ProcessorAudioSignalData.h"

namespace Auvi
{

ProcessorAudioSignalData::ProcessorAudioSignalData()
{
    m_type = AudioSignal;

    m_sampleRate = 0;
}

ProcessorAudioSignalData::~ProcessorAudioSignalData()
{

}

void ProcessorAudioSignalData::SetChannelCount(unsigned long count)
{
    setSize(count, getNumSamples(), true, false, true);
}

unsigned long ProcessorAudioSignalData::GetChannelCount()
{
    return getNumChannels();
}

void ProcessorAudioSignalData::SetSampleRate(unsigned long rate)
{
    m_sampleRate = rate;
}

unsigned long ProcessorAudioSignalData::GetSampleRate()
{
    return m_sampleRate;
}

}