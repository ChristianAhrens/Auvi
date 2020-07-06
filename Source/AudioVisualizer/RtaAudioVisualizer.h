/*
  ==============================================================================

    RtaAudioVisualizer.h
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
    int                 m_plotChannel{ 1 };
    std::vector<float>  m_plotPointsPeak;
    std::vector<float>  m_plotPointsHold;
    int                 m_minFreq{ 0 };
    int                 m_maxFreq{ 0 };
    int                 m_freqRes{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RtaAudioVisualizer)
};

}