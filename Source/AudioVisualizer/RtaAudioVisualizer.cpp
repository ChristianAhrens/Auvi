/*
  ==============================================================================

    RtaAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "RtaAudioVisualizer.h"

//==============================================================================
RtaAudioVisualizer::RtaAudioVisualizer()
    : AbstractAudioVisualizer()
{
    showConfigButton(true);

    m_plotChannel = 1;
    m_channelMapping = { {"Analyzer channel", m_plotChannel}, };
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
    auto maxFreq = 20000.0f;
    auto minFreq = 10;

    Rectangle<int> visuArea(outerMargin, outerMargin, visuAreaWidth, visuAreaHeight);

    // fill our visualization area background
    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    g.fillRect(visuArea);

    auto visuAreaOrigX = float(outerMargin);
    auto visuAreaOrigY = float(outerMargin + visuAreaHeight);

    // draw rta curve
    g.setColour(Colours::forestgreen);
    if (!m_plotPoints.empty())
    {
        auto plotStepFraction = m_plotPoints.size() > 0 ? 1.0f / float(m_plotPoints.size()) : 1.0f;
        auto plotStepWidth = float(visuAreaWidth) * plotStepFraction;

        auto path = Path{};
        auto newPointX = visuAreaOrigX;
        auto newPointY = visuAreaOrigY - m_plotPoints.front() * visuAreaHeight;
        path.startNewSubPath(juce::Point<float>(newPointX, newPointY));
        for (int i = 1; i < m_plotPoints.size(); ++i)
        {
            newPointX = visuAreaOrigX + plotStepWidth * i;
            newPointY = visuAreaOrigY - m_plotPoints.at(i) * visuAreaHeight;

            path.lineTo(juce::Point<float>(newPointX, newPointY));
        }
        g.strokePath(path, PathStrokeType(3));
    }

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
    m_plotChannel = m_channelMapping.at("Analyzer channel");
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

        // we want to statically use 10Hz - 20kHz frequency range, therefor we need to prepare the fft spectrum accordingly
        auto lowestBandInRange = static_cast<int>(10 / spectrum.freqRes);
        auto highestBandInRange = static_cast<int>(20000 / spectrum.freqRes);

        if (lowestBandInRange < 0)
            lowestBandInRange = 0;
        if (highestBandInRange >= spectrum.count)
            highestBandInRange = spectrum.count - 1;

        auto spectrumBandsInRange = highestBandInRange - lowestBandInRange;

        if(m_plotPoints.size() != spectrumBandsInRange)
            m_plotPoints.resize(spectrumBandsInRange);
        memcpy(&m_plotPoints[0], &spectrum.bands[lowestBandInRange], spectrumBandsInRange * sizeof(float));

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
