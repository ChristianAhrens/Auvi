/*
  ==============================================================================

    AbstractAudioVisualizer.h
    Created: 1 Nov 2019 11:39:06am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../Processor/Processor.h"

//==============================================================================
/*
*/
class AbstractAudioVisualizer : public Component, public Processor::Listener
{
public:
    enum VisuType
    {
        InvalidFirst,
        MultiMeter,
        Scope,
        TwoDField,
        Rta,
        Waterfall,
        Waveform,
        InvalidLast
    };
    
public:
    AbstractAudioVisualizer();
    ~AbstractAudioVisualizer();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    virtual VisuType getType() = 0;
    
    //==============================================================================
    static std::string VisuTypeToString(VisuType type);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AbstractAudioVisualizer)
};
