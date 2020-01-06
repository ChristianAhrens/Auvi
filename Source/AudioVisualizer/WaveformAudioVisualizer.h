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

private:
    std::unique_ptr<AudioThumbnail>         m_thumbnail;
    std::unique_ptr<AudioThumbnailCache>    m_thumbnailCache;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformAudioVisualizer)
};
