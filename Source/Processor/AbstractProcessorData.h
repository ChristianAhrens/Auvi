/*
  ==============================================================================

    ProcessorData.h
    Created: 5 Nov 2019 5:17:20pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

class AbstractProcessorData
{
public:
    enum Type
    {
        Invalid,
        Level,
        Spectrum,
    };
    
    AbstractProcessorData();
    ~AbstractProcessorData();
    
    Type GetDataType();
    
protected:
    Type    m_type;
};
