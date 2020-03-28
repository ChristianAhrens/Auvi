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
        float newPointX = visuAreaOrigX;
        float newPointY = visuAreaOrigY - m_plotPoints.front() * visuAreaHeight;
        float plotStepWidth = m_plotPoints.size() > 0 ? float(visuAreaWidth) / float(m_plotPoints.size() - 1) : 1;

        Path path;
        path.startNewSubPath(juce::Point<float>(newPointX, newPointY));
        for (int i = 1; i < m_plotPoints.size(); ++i)
        {
            newPointX += plotStepWidth;
            newPointY = visuAreaOrigY - m_plotPoints.at(i) * visuAreaHeight;

            path.lineTo(juce::Point<float>(newPointX, newPointY));
        }
        g.strokePath(path, PathStrokeType(3));
    }

    // draw a simple baseline
    g.setColour(Colours::white);
    g.drawLine(Line<float>(visuAreaOrigX, visuAreaOrigY, visuAreaOrigX + visuAreaWidth, visuAreaOrigY));
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
