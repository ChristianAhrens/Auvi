/*
  ==============================================================================

    ProcessorLevelData.h
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AbstractProcessorData.h"

class ProcessorAudioSignalData :    public AbstractProcessorData,
                                    public AudioBuffer<float>
{
public:
	ProcessorAudioSignalData();
    ~ProcessorAudioSignalData();
    
    void SetChannelCount(unsigned long count) override;
    unsigned long GetChannelCount() override;
    
};
