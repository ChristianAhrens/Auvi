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
    m_buffer = std::make_unique<RingBuffer<GLfloat>>(2,441);
    m_3Dspectrum = std::make_unique<Spectrum>(m_buffer.get());
    addAndMakeVisible(m_3Dspectrum.get());

    showConfigButton(true);
    setConfigFeatures(AudioVisualizerConfigBase::ConfigFeatures::ChannelMapping);
    setChannelMapping({ {AudioVisualizerConfigBase::MappingKey::WFRTA, m_waterfallPlotChannel}, });
    setUsesValuesInDB(true);
    
    m_3Dspectrum->setChannel(m_waterfallPlotChannel);
    m_3Dspectrum->start();
}

WaterfallAudioVisualizer::~WaterfallAudioVisualizer()
{
    m_3Dspectrum->stop();
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
    if (getChannelMapping().count(AudioVisualizerConfigBase::MappingKey::WFRTA))
        m_waterfallPlotChannel = getChannelMapping().at(AudioVisualizerConfigBase::MappingKey::WFRTA);

    m_3Dspectrum->setChannel(m_waterfallPlotChannel);
}

void WaterfallAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if (!data)
        return;

    switch (data->GetDataType())
    {
    case AbstractProcessorData::AudioSignal:
    {
        ProcessorAudioSignalData *ld = static_cast<ProcessorAudioSignalData*>(data);
        if (ld && ld->GetChannelCount() > 0)
        {
            juce::AudioSampleBuffer* b = static_cast<juce::AudioSampleBuffer*>(ld);
            m_buffer->writeSamples(*b, 0, b->getNumSamples());
            notifyChanges();
        }
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

void WaterfallAudioVisualizer::setPauseProcessing(bool pause)
{
    if (m_3Dspectrum)
    {
        if (pause)
            m_3Dspectrum->stop();
        else
            m_3Dspectrum->start();
    }
}

}
