/*
  ==============================================================================

    ProcessorSpectrumData.h
    Created: 5 Nov 2019 6:14:46pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "AbstractProcessorData.h"

namespace Auvi
{

class ProcessorSpectrumData : public AbstractProcessorData
{
public:
    struct SpectrumBands
    {
        enum {
            count = 1024
        };

        SpectrumBands()
        {
            for (int i = 0; i < count; ++i)
            {
                bandsPeak[i] = 0.0f;
                bandsHold[i] = 0.0f;
            }
            mindB = 0.0f;
            maxdB = 0.0f;
            minFreq = 0.0f;
            maxFreq = 0.0f;
            freqRes = 0.0f;
        };
        
        float   bandsPeak[count];
        float   bandsHold[count];
        float   mindB;
        float   maxdB;
        float   minFreq;
        float   maxFreq;
        float   freqRes;
    };
    
public:
    ProcessorSpectrumData();
    ~ProcessorSpectrumData();
    
    void SetSpectrum(unsigned long channel, SpectrumBands spectrum);
    const SpectrumBands& GetSpectrum(unsigned long channel);
    
    void SetChannelCount(unsigned long count) override;
    unsigned long GetChannelCount() override;
    
private:
    std::map<unsigned long, SpectrumBands>    m_spectrumsMap;

};

}