/*
  ==============================================================================

    ProcessorLevelData.h
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "AbstractProcessorData.h"

class ProcessorLevelData : public AbstractProcessorData
{
public:
    struct LevelVal
    {
        LevelVal()
        {
            peak = 0.0f;
            rms = 0.0f;
        }
        LevelVal(float p, float r)
        {
            peak = p;
            rms = r;
        }
        
        float   peak;
        float   rms;
    };
    
public:
    ProcessorLevelData();
    ~ProcessorLevelData();
    
    void SetLevel(unsigned long channel, LevelVal level);
    LevelVal GetLevel(unsigned long channel);
    
    void SetChannelCount(unsigned long count) override;
    unsigned long GetChannelCount() override;
    
private:
    std::map<unsigned long, LevelVal>    m_levelMap;
};
