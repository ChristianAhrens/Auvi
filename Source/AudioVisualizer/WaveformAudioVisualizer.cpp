/*
  ==============================================================================

    WaveformAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "WaveformAudioVisualizer.h"

namespace Auvi
{

constexpr int THUMB_RES = 512;
constexpr int THUMB_TIME = 10;

//==============================================================================
WaveformAudioVisualizer::WaveformAudioVisualizer()
    : AbstractAudioVisualizer()
{
    AudioFormatManager formatManager;
    formatManager.registerFormat(new WavAudioFormat, true);

    m_thumbnailCache = std::make_unique<AudioThumbnailCache>(THUMB_RES * 1000);
    m_thumbnail = std::make_unique<AudioThumbnail>(THUMB_RES, formatManager, *m_thumbnailCache.get());

    m_buffer.clear();
    m_bufferPos = 0;
    m_bufferTime = 0;

    setUsesValuesInDB(false);
}

WaveformAudioVisualizer::~WaveformAudioVisualizer()
{
}

void WaveformAudioVisualizer::paint(Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    auto outerMargin = 20.0f;

    Rectangle<int> visuArea = getLocalBounds().reduced(outerMargin);

    // fill our visualization area background
    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    g.fillRect(visuArea);

    if (m_thumbnail->getTotalLength() > 0.0)
    {
        g.setColour(Colours::forestgreen.darker());
        m_thumbnail->drawChannels(g, visuArea, 0, THUMB_TIME, 1.0f);
    }
    else
    {
        g.setColour(Colours::white);
        g.setFont(14.0f);
        g.drawFittedText("(No waveform data to paint)", visuArea, Justification::centred, 2);
    }

    // draw moving cursor
    if (m_bufferTime > 0)
    {
        g.setColour(Colours::white);
        auto cursorPos = visuArea.getWidth() * (float(m_bufferPos) / float(m_bufferTime));
        g.drawRect(Rectangle<int>(visuArea.getX() + cursorPos, visuArea.getY(), 1, visuArea.getHeight()));
    }

    // draw an outline around the visu area
    g.setColour(Colours::white);
    g.drawRect(visuArea, 1);

    // draw legend, simply number the waveforms from 1..n
    if (m_thumbnail->getNumChannels() > 0)
    {
        g.setFont(14.0f);
        auto thumbWaveHeight = visuArea.getHeight() / m_thumbnail->getNumChannels();
        auto yPos = outerMargin / 2 + thumbWaveHeight / 2;
        for (unsigned long i = 1; i <= m_thumbnail->getNumChannels(); ++i)
        {
            g.setColour(Colours::white);
            g.drawText(String(i), juce::Rectangle<float>(0, yPos, outerMargin, outerMargin), Justification::centred, true);

            yPos += thumbWaveHeight;
        }
    }
}

void WaveformAudioVisualizer::resized()
{
    AbstractAudioVisualizer::resized();
}

AbstractAudioVisualizer::VisuType WaveformAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Waveform;
}

void WaveformAudioVisualizer::processChangedChannelMapping()
{

}

void WaveformAudioVisualizer::processingDataChanged(AbstractProcessorData* data)
{
    if (!data)
        if (!data)
            return;

    switch (data->GetDataType())
    {
    case AbstractProcessorData::AudioSignal:
    {
        ProcessorAudioSignalData* sd = static_cast<ProcessorAudioSignalData*>(data);
        if (sd->GetChannelCount() > 0)
        {
            m_bufferTime = static_cast<int>(THUMB_TIME * sd->GetSampleRate());

            if (m_thumbnail->getNumChannels() != sd->GetChannelCount())
                m_thumbnail->reset(static_cast<int>(sd->GetChannelCount()), sd->GetSampleRate());
            if(m_buffer.getNumChannels() != sd->GetChannelCount())
                m_buffer.setSize(static_cast<int>(sd->GetChannelCount()), m_bufferTime, false, true, true);

            for (int i = 0; i < m_buffer.getNumChannels(); ++i)
            {
                m_buffer.copyFrom(i, m_bufferPos, *sd, i, 0, sd->getNumSamples());
            }
            m_bufferPos += sd->getNumSamples();

            if (m_bufferPos >= m_bufferTime)
                m_bufferPos = 0;

            m_thumbnail->addBlock(0, m_buffer, 0, m_bufferTime);
        }
        else
            break;
        notifyChanges();
        break;
    }
    case AbstractProcessorData::Level:
    case AbstractProcessorData::Spectrum:
    case AbstractProcessorData::Invalid:
    default:
        break;
    }
}

}
