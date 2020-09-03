/*
  ==============================================================================

    WaterfallAudioVisualizer.h
    Created: 1 Nov 2019 11:43:00am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../AbstractAudioVisualizer.h"

#include "RingBuffer.h"
#include "Spectrum.h"

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
    void setPauseProcessing(bool pause) override;
    
    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    int m_waterfallPlotChannel{ 1 };
    std::unique_ptr<RingBuffer<GLfloat>> m_buffer;
    std::unique_ptr<Spectrum>   m_3Dspectrum;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterfallAudioVisualizer)
};

}
