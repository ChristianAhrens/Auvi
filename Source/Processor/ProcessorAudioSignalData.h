/*
  ==============================================================================

    ProcessorLevelData.h
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AbstractProcessorData.h"

namespace Auvi
{

class ProcessorAudioSignalData :    public AbstractProcessorData,
                                    public AudioBuffer<float>
{
public:
	ProcessorAudioSignalData();
    ~ProcessorAudioSignalData();
    
    void SetChannelCount(unsigned long count) override;
    unsigned long GetChannelCount() override;

    void SetSampleRate(unsigned long rate);
    unsigned long GetSampleRate();

private:
    unsigned long m_sampleRate;
    
};

}