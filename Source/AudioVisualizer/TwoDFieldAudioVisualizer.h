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
    unsigned long   m_channelL;
    unsigned long   m_channelC;
    unsigned long   m_channelR;
    unsigned long   m_channelLS;
    unsigned long   m_channelRS;
    ProcessorLevelData  m_levelData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoDFieldAudioVisualizer)
};

}