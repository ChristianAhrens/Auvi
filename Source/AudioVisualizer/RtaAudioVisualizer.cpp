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
    m_plotChannel = 0;
}

RtaAudioVisualizer::~RtaAudioVisualizer()
{
}

void RtaAudioVisualizer::paint (Graphics& g)
{
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
            auto plotPointFreq = maxFreq * plotStepFraction * i;
            newPointX = visuAreaOrigX + visuAreaWidth * (log10(plotPointFreq)/log10(maxFreq));
            newPointY = visuAreaOrigY - m_plotPoints.at(i) * visuAreaHeight;

            path.lineTo(juce::Point<float>(newPointX, newPointY));
        }
        g.strokePath(path, PathStrokeType(3));
    }

    g.setColour(Colours::white);
    // draw marker lines 10Hz, 100Hz, 1000Hz, 10000Hz
    auto quarterVisuArea = visuAreaWidth * 0.25f;
    for(int i=0; i<4; ++i)
    {
        auto quarterOrig = visuAreaOrigX + (quarterVisuArea * i);
        for (int j = 1; j < 11; ++j)
        {
            auto quarterOffset = quarterVisuArea * log10(j);
            g.drawLine(Line<float>( quarterOrig + quarterOffset, visuAreaOrigY,
                                    quarterOrig + quarterOffset, visuAreaOrigY - visuAreaHeight));
        }
    }
    // draw an outline around the visu area
    g.drawRect(visuArea, 1);
}

void RtaAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType RtaAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Rta;
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
        if (spectrumData.GetChannelCount() < m_plotChannel)
            break;

        if(m_plotPoints.size() != ProcessorSpectrumData::SpectrumBands::count)
            m_plotPoints.resize(ProcessorSpectrumData::SpectrumBands::count);
        memcpy(&m_plotPoints[0], &spectrumData.GetSpectrum(m_plotChannel).bands[0], ProcessorSpectrumData::SpectrumBands::count * sizeof(float));
        repaint(); 
        }
        break;
    case AbstractProcessorData::Level:
    case AbstractProcessorData::AudioSignal:
    case AbstractProcessorData::Invalid:
    default:
        break;
    }
}
