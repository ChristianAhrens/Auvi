/*
  ==============================================================================

    ProcessorSpectrumData.cpp
    Created: 5 Nov 2019 6:14:46pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ProcessorSpectrumData.h"

namespace Auvi
{

ProcessorSpectrumData::ProcessorSpectrumData()
{
    m_type = Spectrum;
}

ProcessorSpectrumData::~ProcessorSpectrumData()
{

}

void ProcessorSpectrumData::SetSpectrum(unsigned long channel, ProcessorSpectrumData::SpectrumBands spectrum)
{
    m_spectrumsMap[channel] = spectrum;
}

const ProcessorSpectrumData::SpectrumBands& ProcessorSpectrumData::GetSpectrum(unsigned long channel)
{
    if(m_spectrumsMap.count(channel) == 0)
        m_spectrumsMap.insert(std::pair<unsigned long, SpectrumBands>(channel, ProcessorSpectrumData::SpectrumBands{}));

    return m_spectrumsMap.at(channel);
}

void ProcessorSpectrumData::SetChannelCount(unsigned long count)
{
    if(GetChannelCount()==count)
        return;
    
    for(unsigned long i=1; i<=count; i++)
    {
        if(!m_spectrumsMap.count(i))
            m_spectrumsMap[i] = SpectrumBands();
    }
}

unsigned long ProcessorSpectrumData::GetChannelCount()
{
    return m_spectrumsMap.size();
}

}