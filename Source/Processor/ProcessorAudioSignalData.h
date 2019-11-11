/*
  ==============================================================================

    ProcessorLevelData.h
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "AbstractProcessorData.h"

class ProcessorAudioSignalData : public AbstractProcessorData
{
public:
    struct SignalVal
    {
		SignalVal()
        {
            peak = 0.0f;
            rms = 0.0f;
        }
		SignalVal(float p, float r)
        {
            peak = p;
            rms = r;
        }
        
        float   peak;
        float   rms;
    };
    
public:
	ProcessorAudioSignalData();
    ~ProcessorAudioSignalData();
    
    void SetAudioSignal(unsigned long channel, SignalVal level);
	SignalVal GetAudioSignal(unsigned long channel);
    
    void SetChannelCount(unsigned long count) override;
    unsigned long GetChannelCount() override;
    
private:
    std::map<unsigned long, SignalVal>    m_signalMap;
};
