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
            peakdB = 0.0f;
            rmsdB = 0.0f;
        }
        LevelVal(float p, float r, float infdb = -100.0f)
        {
            peak = p;
            rms = r;
            peakdB = Decibels::gainToDecibels(peak, infdb);
            rmsdB = Decibels::gainToDecibels(rms, infdb);
            minusInfdb = infdb;
        }

        float GetFactorRMSdB()
        {
            return (-1 * minusInfdb + rmsdB) / (-1 * minusInfdb);
        }
        float GetFactorPEAKdB()
        {
            return (-1 * minusInfdb + peakdB) / (-1 * minusInfdb);
        }
        
        float   peak;
        float   rms;
        float   peakdB;
        float   rmsdB;
        float   minusInfdb;
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
