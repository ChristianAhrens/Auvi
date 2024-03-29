/*
  ==============================================================================

    MultiMeterAudioVisualizer.h
    Created: 1 Nov 2019 11:43:48am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AbstractAudioVisualizer.h"

namespace Auvi
{

//==============================================================================
/*
*/
class MultiMeterAudioVisualizer    : public AbstractAudioVisualizer
{
public:
    MultiMeterAudioVisualizer();
    ~MultiMeterAudioVisualizer();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void processingDataChanged(AbstractProcessorData *data) override;
    
    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    ProcessorLevelData  m_levelData;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiMeterAudioVisualizer)
};

}