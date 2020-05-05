/*
  ==============================================================================

    WaveformAudioVisualizer.h
    Created: 1 Nov 2019 11:43:00am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "AbstractAudioVisualizer.h"

namespace Auvi
{

//==============================================================================
/*
*/
class WaveformAudioVisualizer    : public AbstractAudioVisualizer
{
public:
    WaveformAudioVisualizer();
    ~WaveformAudioVisualizer();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void processingDataChanged(AbstractProcessorData *data) override;
    
    //==============================================================================
    VisuType getType() override;
    void processChangedChannelMapping() override;

private:
    std::unique_ptr<AudioThumbnail>         m_thumbnail;
    std::unique_ptr<AudioThumbnailCache>    m_thumbnailCache;

    AudioBuffer<float>                      m_buffer;
    int                                     m_bufferPos;
    int                                     m_bufferTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformAudioVisualizer)
};

}