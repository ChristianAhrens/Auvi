/*
  ==============================================================================

    RtaAudioVisualizer.h
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
class RtaAudioVisualizer    : public AbstractAudioVisualizer
{
public:
    RtaAudioVisualizer();
    ~RtaAudioVisualizer();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void processingDataChanged(AbstractProcessorData *data) override;
    
    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    int                 m_plotChannel;
    std::vector<float>  m_plotPointsPeak;
    std::vector<float>  m_plotPointsHold;
    int                 m_minFreq;
    int                 m_maxFreq;
    int                 m_freqRes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RtaAudioVisualizer)
};
