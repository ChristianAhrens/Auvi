/*
  ==============================================================================

    ProcessorLevelData.cpp
    Created: 5 Nov 2019 6:14:35pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ProcessorLevelData.h"

ProcessorLevelData::ProcessorLevelData()
{
    m_type = Level;
}

ProcessorLevelData::~ProcessorLevelData()
{

}

void ProcessorLevelData::SetLevel(unsigned long channel, ProcessorLevelData::LevelVal level)
{
    m_levelMap[channel] = level;
}

ProcessorLevelData::LevelVal ProcessorLevelData::GetLevel(unsigned long channel)
{
    if(m_levelMap.count(channel))
        return m_levelMap.at(channel);
    else
        return ProcessorLevelData::LevelVal(0,0,0);
}

void ProcessorLevelData::SetChannelCount(unsigned long count)
{
    if(GetChannelCount()==count)
        return;
    
    for(unsigned long i=1; i<=count; i++)
    {
        if (!m_levelMap.count(i))
            m_levelMap[i] = LevelVal(0, 0, 0);
    }
}

unsigned long ProcessorLevelData::GetChannelCount()
{
    return m_levelMap.size();
}
