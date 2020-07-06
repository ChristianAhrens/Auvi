/*
  ==============================================================================

    WaterfallAudioVisualizer.h
    Created: 1 Nov 2019 11:43:00am
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
class WaterfallAudioVisualizer    : public AbstractAudioVisualizer
{
public:
    WaterfallAudioVisualizer();
    ~WaterfallAudioVisualizer();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void processingDataChanged(AbstractProcessorData *data) override;
    
    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterfallAudioVisualizer)
};

}