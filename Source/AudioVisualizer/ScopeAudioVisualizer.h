/*
  ==============================================================================

    ScopeAudioVisualizer.h
    Created: 1 Nov 2019 11:43:27am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "AbstractAudioVisualizer.h"

//==============================================================================
/*
*/
class ScopeAudioVisualizer    : public AbstractAudioVisualizer
{
public:
    ScopeAudioVisualizer();
    ~ScopeAudioVisualizer();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopeAudioVisualizer)
};
