/*
  ==============================================================================

    WaterfallAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "WaterfallAudioVisualizer.h"

namespace Auvi
{

//==============================================================================
WaterfallAudioVisualizer::WaterfallAudioVisualizer()
    : AbstractAudioVisualizer()
{
    m_buffer = std::make_unique<RingBuffer>(0,0);
    m_3Dspectrum = std::make_unique<Spectrum>(m_buffer.get());
    addAndMakeVisible(m_3Dspectrum.get());
}

WaterfallAudioVisualizer::~WaterfallAudioVisualizer()
{
}

void WaterfallAudioVisualizer::paint (Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("WaterfallAudioVisualizer", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void WaterfallAudioVisualizer::resized()
{
    AbstractAudioVisualizer::resized();
}

AbstractAudioVisualizer::VisuType WaterfallAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Waterfall;
}

void WaterfallAudioVisualizer::processChangedChannelMapping()
{

}

void WaterfallAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if (!data)
        if (!data)
            return;

    switch (data->GetDataType())
    {
    case AbstractProcessorData::AudioSignal:
    case AbstractProcessorData::Level:
    case AbstractProcessorData::Spectrum:
    case AbstractProcessorData::Invalid:
    default:
        break;
    }
}

}
