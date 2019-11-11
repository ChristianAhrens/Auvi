/*
  ==============================================================================

    ProcessorData.h
    Created: 5 Nov 2019 5:17:20pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AbstractProcessorData
{
public:
    enum Type
    {
        Invalid,
		AudioSignal,
        Level,
        Spectrum,
    };
    
    AbstractProcessorData();
    virtual ~AbstractProcessorData();
    
    Type GetDataType();
    
    virtual void SetChannelCount(unsigned long count) = 0;
    virtual unsigned long GetChannelCount() = 0;
    
protected:
    Type    m_type;
};
