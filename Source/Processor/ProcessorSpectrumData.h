/*
  ==============================================================================

    ProcessorSpectrumData.h
    Created: 5 Nov 2019 6:14:46pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "AbstractProcessorData.h"

class ProcessorSpectrumData : public AbstractProcessorData
{
public:
    struct SpectrumBands
    {
        SpectrumBands()
        {
            for(int i=0; i<64; ++i)
                bands[i] = 0.0f;
        }
        
        float   bands[64];
    };
    
public:
    ProcessorSpectrumData();
    ~ProcessorSpectrumData();
    
    void SetSpectrum(int channel, SpectrumBands spectrum);
    SpectrumBands GetSpectrum(int channel);
    
    void SetChannelCount(int count) override;
    unsigned long GetChannelCount() override;
    
private:
    std::map<int, SpectrumBands>    m_spectrumsMap;

};
