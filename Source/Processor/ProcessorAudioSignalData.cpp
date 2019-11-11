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

void ProcessorAudioSignalData::SetAudioSignal(unsigned long channel, ProcessorAudioSignalData::SignalVal signal)
{
    m_signalMap[channel] = signal;
}

ProcessorAudioSignalData::SignalVal ProcessorAudioSignalData::GetAudioSignal(unsigned long channel)
{
    if(m_signalMap.count(channel))
        return m_signalMap.at(channel);
    else
        return ProcessorAudioSignalData::SignalVal(0,0);
}

void ProcessorAudioSignalData::SetChannelCount(unsigned long count)
{
    if(GetChannelCount()==count)
        return;
    
    for(unsigned long i=1; i<=count; i++)
    {
        if(!m_signalMap.count(i))
            m_signalMap[i] = SignalVal(0,0);
    }
}

unsigned long ProcessorAudioSignalData::GetChannelCount()
{
    return m_signalMap.size();
}
