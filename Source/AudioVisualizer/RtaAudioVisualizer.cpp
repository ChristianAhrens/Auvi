/*
  ==============================================================================

    RtaAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "RtaAudioVisualizer.h"

#include "../utils.hpp"

namespace Auvi
{

//==============================================================================
RtaAudioVisualizer::RtaAudioVisualizer()
    : AbstractAudioVisualizer()
{
    showConfigButton(true);

    m_minFreq = 0;
    m_maxFreq = 0;
    m_freqRes = 0;

    m_plotChannel = 1;
    m_channelMapping = { {AudioVisualizerConfigBase::MappingKey::RTA, m_plotChannel}, };
}

RtaAudioVisualizer::~RtaAudioVisualizer()
{
}

void RtaAudioVisualizer::paint (Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // calculate what we need for our center circle
    auto width = getWidth();
    auto height = getHeight();
    auto outerMargin = 20;
    auto visuAreaWidth = width - 2 * outerMargin;
    auto visuAreaHeight = height - 2 * outerMargin;
    auto maxPlotFreq = 20000;
    auto minPlotFreq = 10;

    Rectangle<int> visuArea(outerMargin, outerMargin, visuAreaWidth, visuAreaHeight);

    // fill our visualization area background
    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    g.fillRect(visuArea);

    auto visuAreaOrigX = float(outerMargin);
    auto visuAreaOrigY = float(outerMargin + visuAreaHeight);

    // draw rta curve
    if (!m_plotPointsPeak.empty() && m_plotPointsHold.size() == m_plotPointsPeak.size())
    {
        auto minPlotIdx = jlimit(0, static_cast<int>(m_plotPointsPeak.size() - 1), (minPlotFreq - m_minFreq) / m_freqRes);
        auto maxPlotIdx = jlimit(0, static_cast<int>(m_plotPointsPeak.size() - 1), (maxPlotFreq - m_minFreq) / m_freqRes);

        // hold curve
        auto path = Path{};
        auto skewedProportionX = 1.0f / (log10(maxPlotFreq) - 1.0f) * (log10((minPlotIdx + 1) * m_freqRes) - 1.0f);
        auto newPointX = visuAreaOrigX + (static_cast<float>(visuAreaWidth) * skewedProportionX);
        auto newPointY = visuAreaOrigY - m_plotPointsHold.at(minPlotIdx) * visuAreaHeight;
        path.startNewSubPath(juce::Point<float>(newPointX, newPointY));
        for (int i = minPlotIdx + 1; i <= maxPlotIdx; ++i)
        {
            skewedProportionX = 1.0f / (log10(maxPlotFreq) - 1.0f) * (log10((i + 1) * m_freqRes) - 1.0f);
            newPointX = visuAreaOrigX + (static_cast<float>(visuAreaWidth) * skewedProportionX);
            newPointY = visuAreaOrigY - m_plotPointsHold.at(i) * visuAreaHeight;

            path.lineTo(juce::Point<float>(newPointX, newPointY));
        }
        g.setColour(Colours::grey);
        g.strokePath(path, PathStrokeType(1));

        // peak curve
        path = Path{};
        skewedProportionX = 1.0f / (log10(maxPlotFreq) - 1.0f) * (log10((minPlotIdx + 1) * m_freqRes) - 1.0f);
        newPointX = visuAreaOrigX + (static_cast<float>(visuAreaWidth) * skewedProportionX);
        newPointY = visuAreaOrigY - m_plotPointsPeak.at(minPlotIdx) * visuAreaHeight;
        path.startNewSubPath(juce::Point<float>(newPointX, newPointY));
        for (int i = minPlotIdx + 1; i <= maxPlotIdx; ++i)
        {
            skewedProportionX = 1.0f / (log10(maxPlotFreq) - 1.0f) * (log10((i + 1) * m_freqRes) - 1.0f);
            newPointX = visuAreaOrigX + (static_cast<float>(visuAreaWidth) * skewedProportionX);
            newPointY = visuAreaOrigY - m_plotPointsPeak.at(i) * visuAreaHeight;

            path.lineTo(juce::Point<float>(newPointX, newPointY));
        }
        g.setColour(Colours::forestgreen);
        g.strokePath(path, PathStrokeType(3));
    }

    // draw dBFS
    g.setFont(12.0f);
    g.setColour(Colours::grey);
    g.drawText(String(Auvi::utils::getGlobalMindB()) + " ... " + String(Auvi::utils::getGlobalMaxdB()) + " dBFS", Rectangle<float>(visuAreaOrigX + visuAreaWidth - 100.0f, float(outerMargin), 110.0f, float(outerMargin)), Justification::centred, true);

    g.setColour(Colours::white);
    // draw marker lines 10Hz, 100Hz, 1000Hz, 10000Hz
    auto markerLineValues = std::vector<float>{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 20000};
    auto markerLegendValues = std::map<int, std::string>{ {10, "10"}, {100, "100"}, {1000, "1k"}, {10000, "10k"}, {20000, "20k"} };
    auto legendValueWidth = 40.0f;
    for (auto i = 0; i < markerLineValues.size(); ++i)
    {
        auto skewedProportionX = 1.0f / (log10(markerLineValues.back()) - 1.0f) * (log10(markerLineValues.at(i)) - 1.0f);
        auto posX = visuAreaOrigX + (static_cast<float>(visuAreaWidth) * skewedProportionX);
        g.drawLine(Line<float>( posX, visuAreaOrigY, posX, visuAreaOrigY - visuAreaHeight));

        if(markerLegendValues.count(markerLineValues.at(i)))
            g.drawText(markerLegendValues.at(markerLineValues.at(i)), Rectangle<float>(posX - 0.5f * legendValueWidth, visuAreaOrigY, legendValueWidth, float(outerMargin)), Justification::centred, true);
    }

    // draw an outline around the visu area
    g.drawRect(visuArea, 1);
}

void RtaAudioVisualizer::resized()
{
    AbstractAudioVisualizer::resized();
}

AbstractAudioVisualizer::VisuType RtaAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Rta;
}

void RtaAudioVisualizer::processChangedChannelMapping()
{
    m_plotChannel = m_channelMapping.at(AudioVisualizerConfigBase::MappingKey::RTA);
}

void RtaAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if (!data)
        return;

    switch (data->GetDataType())
    {
    case AbstractProcessorData::Spectrum:
        {
        ProcessorSpectrumData spectrumData = *(static_cast<ProcessorSpectrumData*>(data));
        if (spectrumData.GetChannelCount() < (m_plotChannel - 1))
            break;

        auto spectrum = spectrumData.GetSpectrum(m_plotChannel - 1);
        if (spectrum.freqRes <= 0)
            break;

        if (m_plotPointsPeak.size() != spectrum.count)
            m_plotPointsPeak.resize(spectrum.count);
        memcpy(&m_plotPointsPeak[0], &spectrum.bandsPeak[0], spectrum.count * sizeof(float));

        if (m_plotPointsHold.size() != spectrum.count)
            m_plotPointsHold.resize(spectrum.count);
        memcpy(&m_plotPointsHold[0], &spectrum.bandsHold[0], spectrum.count * sizeof(float));

        m_minFreq = spectrum.minFreq;
        m_maxFreq = spectrum.maxFreq;
        m_freqRes = spectrum.freqRes;

        notifyChanges();
        }
        break;
    case AbstractProcessorData::Level:
    case AbstractProcessorData::AudioSignal:
    case AbstractProcessorData::Invalid:
    default:
        break;
    }
}

}
