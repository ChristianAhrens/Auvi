/*
  ==============================================================================

    MultiMeterAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:48am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MultiMeterAudioVisualizer.h"

//==============================================================================
MultiMeterAudioVisualizer::MultiMeterAudioVisualizer()
    : AbstractAudioVisualizer()
{

}

MultiMeterAudioVisualizer::~MultiMeterAudioVisualizer()
{
}

void MultiMeterAudioVisualizer::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	// calculate what we need for our center circle
	auto width = getWidth();
	auto height = getHeight();
	auto outerMargin = 20;
    auto maxMeterWidth = 30;
	auto visuAreaWidth = width - 2 * outerMargin;
	auto visuAreaHeight = height - 2 * outerMargin;

	Rectangle<int> visuArea(outerMargin, outerMargin, visuAreaWidth, visuAreaHeight);

	// fill our visualization area background
	g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
	g.fillRect(visuArea);

	auto visuAreaOrigX = float(outerMargin);
	auto visuAreaOrigY = float(outerMargin + visuAreaHeight);

	// draw a simple baseline
	g.setColour(Colours::white);
	g.drawLine(Line<float>(visuAreaOrigX, visuAreaOrigY, visuAreaOrigX + visuAreaWidth, visuAreaOrigY));

	// draw dummy meters
    auto meterSpacing = outerMargin * 0.5f;
    auto meterWidth = (visuArea.getWidth() - (m_levelData.GetChannelCount() + 1) * meterSpacing) / m_levelData.GetChannelCount();
    meterWidth = meterWidth > maxMeterWidth ? maxMeterWidth : meterWidth;
    auto meterMaxHeight = visuArea.getHeight() - 2 * meterSpacing;
    auto meterLeft = visuAreaOrigX + meterSpacing;

    g.setFont(14.0f);
    for(unsigned long i=1; i<=m_levelData.GetChannelCount(); ++i)
    {
        auto rmsMeterHeight = meterMaxHeight * m_levelData.GetLevel(i).rms;
        auto peakMeterHeight = rmsMeterHeight - (meterMaxHeight * m_levelData.GetLevel(i).peak);

        g.setColour(Colours::forestgreen);
        g.fillRect(Rectangle<float>(meterLeft, visuAreaOrigY - rmsMeterHeight, meterWidth, rmsMeterHeight));
        g.setColour(Colours::forestgreen.darker());
        g.fillRect(Rectangle<float>(meterLeft, visuAreaOrigY - rmsMeterHeight - peakMeterHeight, meterWidth, peakMeterHeight));
		g.setColour(Colours::white);
        g.drawText(String(i), Rectangle<float>(meterLeft, visuAreaOrigY, meterWidth, float(outerMargin)), Justification::centred, true);

        meterLeft += meterWidth + meterSpacing;
    }
}

void MultiMeterAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType MultiMeterAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::MultiMeter;
}

void MultiMeterAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if(!data)
        return;
    
    switch(data->GetDataType())
    {
        case AbstractProcessorData::Level:
            m_levelData = *(static_cast<ProcessorLevelData*>(data));
            repaint();
            break;
        case AbstractProcessorData::Spectrum:
            break;
        case AbstractProcessorData::Invalid:
        default:
            break;
    }
}
