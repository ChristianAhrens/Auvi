/*
  ==============================================================================

    ProcessorSpectrumData.cpp
    Created: 5 Nov 2019 6:14:46pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ProcessorSpectrumData.h"

ProcessorSpectrumData::ProcessorSpectrumData()
{
    m_type = Spectrum;
}

ProcessorSpectrumData::~ProcessorSpectrumData()
{

}

void ProcessorSpectrumData::SetSpectrum(int channel, ProcessorSpectrumData::SpectrumBands spectrum)
{
    m_spectrumsMap[channel] = spectrum;
}

ProcessorSpectrumData::SpectrumBands ProcessorSpectrumData::GetSpectrum(int channel)
{
    if(m_spectrumsMap.count(channel))
        return m_spectrumsMap.at(channel);
    else
        return ProcessorSpectrumData::SpectrumBands();
}

void ProcessorSpectrumData::SetChannelCount(int count)
{
    if(GetChannelCount()==count)
        return;
    
    for(int i=1; i<=count; i++)
    {
        if(!m_spectrumsMap.count(i))
            m_spectrumsMap[i] = SpectrumBands();
    }
}

unsigned long ProcessorSpectrumData::GetChannelCount()
{
    return m_spectrumsMap.size();
}
