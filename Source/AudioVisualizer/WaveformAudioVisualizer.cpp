/*
  ==============================================================================

    WaveformAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "WaveformAudioVisualizer.h"

//==============================================================================
WaveformAudioVisualizer::WaveformAudioVisualizer()
    : AbstractAudioVisualizer()
{
    AudioFormatManager formatManager;
    formatManager.registerFormat(new WavAudioFormat, true);

    m_thumbnailCache = std::make_unique<AudioThumbnailCache>(240000);
    m_thumbnail = std::make_unique<AudioThumbnail>(512, formatManager, *m_thumbnailCache.get());
}

WaveformAudioVisualizer::~WaveformAudioVisualizer()
{
}

void WaveformAudioVisualizer::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    if (m_thumbnail->getTotalLength() > 0.0)
    {
        g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
        m_thumbnail->drawChannels(g, getLocalBounds().reduced(2), 0, 5, 1.0f);
    }
    else
    {
        g.setColour(Colours::white);
        g.setFont(14.0f);
        g.drawFittedText("(No waveform data to paint)", getLocalBounds().reduced(2), Justification::centred, 2);
    }
}

void WaveformAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType WaveformAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Waveform;
}

void WaveformAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if (!data)
        if (!data)
            return;

    switch (data->GetDataType())
    {
    case AbstractProcessorData::AudioSignal:
    {
        ProcessorAudioSignalData* ld = static_cast<ProcessorAudioSignalData*>(data);
        if (ld->GetChannelCount() > 0)
        {
            if (m_thumbnail->getNumChannels() != ld->GetChannelCount())
                m_thumbnail->reset(ld->GetChannelCount(), ld->GetSampleRate());

            m_thumbnail->addBlock(0, *ld, 0, ld->getNumSamples());
        }
        else
            break;
        repaint();
        break;
    }
    case AbstractProcessorData::Level:
        break;
    case AbstractProcessorData::Spectrum:
        break;
    case AbstractProcessorData::Invalid:
    default:
        break;
    }
}
