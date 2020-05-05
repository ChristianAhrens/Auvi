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
class ScopeAudioVisualizer : public AbstractAudioVisualizer
{
public:
    ScopeAudioVisualizer();
    ~ScopeAudioVisualizer();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    void processingDataChanged(AbstractProcessorData* data) override;

    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    unsigned long GetNextScopeTailPos();
    juce::Point<float> MapValToRect(float x, float y, Rectangle<float> rectF);

    unsigned long   m_channelX{ 1 };
    unsigned long   m_channelY{ 2 };
    int m_dotDiameter{ 2 };
    unsigned long   m_scopeTailLength{ 70 };
    unsigned long   m_scopeTailPos{ 0 };
    std::vector<std::pair<float, float>>   m_scopeTail;

    float m_min{ 0.0f };
    float m_max{ 1.0f };
    float m_mindB{ -100.0f };
    float m_maxdB{ 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopeAudioVisualizer)
};
