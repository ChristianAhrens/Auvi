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
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void processingDataChanged(AbstractProcessorData *data) override;
    
    //==============================================================================
    VisuType getType() override;

private:
    unsigned long GetNextScopeTailPos();
    Point<float> MapValToRect(float x, float y, Rectangle<float> rectF);
    
    unsigned long   m_channelX;
    unsigned long   m_channelY;
    unsigned long   m_scopeTailLength;
    unsigned long   m_scopeTailPos;
    std::vector<ProcessorAudioSignalData::SignalVal>   m_scopeTailX;
    std::vector<ProcessorAudioSignalData::SignalVal>   m_scopeTailY;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopeAudioVisualizer)
};
