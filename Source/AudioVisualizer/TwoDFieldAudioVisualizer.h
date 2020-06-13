/*
  ==============================================================================

    TwoDFieldAudioVisualizer.h
    Created: 1 Nov 2019 11:43:00am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "AbstractAudioVisualizer.h"

namespace Auvi
{

//==============================================================================
/*
*/
class TwoDFieldAudioVisualizer    : public AbstractAudioVisualizer
{
public:
    TwoDFieldAudioVisualizer();
    ~TwoDFieldAudioVisualizer();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void processingDataChanged(AbstractProcessorData *data) override;
    
    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    unsigned long   m_channelL{ 1 };
    unsigned long   m_channelC{ 2 };
    unsigned long   m_channelR{ 3 };
    unsigned long   m_channelLS{ 5 };
    unsigned long   m_channelRS{ 4 };

    ProcessorLevelData  m_levelData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoDFieldAudioVisualizer)
};

}