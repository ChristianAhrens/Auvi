/*
  ==============================================================================

    WaveformAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "WaveformAudioVisualizer.h"

constexpr int THUMB_RES = 512;
constexpr int THUMB_TIME = 10;

//==============================================================================
WaveformAudioVisualizer::WaveformAudioVisualizer()
    : AbstractAudioVisualizer()
{
    AudioFormatManager formatManager;
    formatManager.registerFormat(new WavAudioFormat, true);

    m_thumbnailCache = std::make_unique<AudioThumbnailCache>(THUMB_RES*1000);
    m_thumbnail = std::make_unique<AudioThumbnail>(THUMB_RES, formatManager, *m_thumbnailCache.get());

    m_buffer.clear();
    m_bufferPos = 0;
    m_bufferTime = 0;
}

WaveformAudioVisualizer::~WaveformAudioVisualizer()
{
}

void WaveformAudioVisualizer::paint (Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    Rectangle<int> visuArea = getLocalBounds().reduced(20);

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
}

void WaveformAudioVisualizer::resized()
{
    AbstractAudioVisualizer::resized();
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
            m_bufferTime = THUMB_TIME * ld->GetSampleRate();

            if (m_thumbnail->getNumChannels() != ld->GetChannelCount())
                m_thumbnail->reset(ld->GetChannelCount(), ld->GetSampleRate());
            if(m_buffer.getNumChannels() != ld->GetChannelCount())
                m_buffer.setSize(ld->GetChannelCount(), m_bufferTime, false, true, true);

            for (int i = 0; i < m_buffer.getNumChannels(); ++i)
            {
                m_buffer.copyFrom(i, m_bufferPos,  *ld, i, 0, ld->getNumSamples());
            }
            m_bufferPos += ld->getNumSamples();

            if (m_bufferPos >= m_bufferTime)
                m_bufferPos = 0;

            m_thumbnail->addBlock(0, m_buffer, 0, m_bufferTime);
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
