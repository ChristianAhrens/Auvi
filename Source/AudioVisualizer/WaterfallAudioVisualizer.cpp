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
    m_buffer = std::make_unique<RingBuffer<GLfloat>>(0,0);
    m_3Dspectrum = std::make_unique<Spectrum>(m_buffer.get());
    addAndMakeVisible(m_3Dspectrum.get());
}

WaterfallAudioVisualizer::~WaterfallAudioVisualizer()
{
}

void WaterfallAudioVisualizer::paint (Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void WaterfallAudioVisualizer::resized()
{
    AbstractAudioVisualizer::resized();

    // calculate what we need for our center circle
    auto width = getWidth();
    auto height = getHeight();
    auto outerMargin = 20;
    auto visuAreaWidth = width - 2 * outerMargin;
    auto visuAreaHeight = height - 2 * outerMargin;
    m_3Dspectrum->setBounds(juce::Rectangle<int>(outerMargin, outerMargin, visuAreaWidth, visuAreaHeight));
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
