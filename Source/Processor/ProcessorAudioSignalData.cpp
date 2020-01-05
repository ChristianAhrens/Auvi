/*
  ==============================================================================

    ProcessorAudioSignalData.cpp
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ProcessorAudioSignalData.h"

ProcessorAudioSignalData::ProcessorAudioSignalData()
{
    m_type = AudioSignal;
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
