/*
  ==============================================================================

    AbstractAudioVisualizer.h
    Created: 1 Nov 2019 11:39:06am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AbstractAudioVisualizer    : public Component
{
public:
    AbstractAudioVisualizer();
    ~AbstractAudioVisualizer();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AbstractAudioVisualizer)
};
