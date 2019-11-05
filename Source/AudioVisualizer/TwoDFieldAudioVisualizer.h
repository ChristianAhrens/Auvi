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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoDFieldAudioVisualizer)
};
