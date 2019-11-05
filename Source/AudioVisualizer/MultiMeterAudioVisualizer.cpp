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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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
    auto maxMeterWidth = 50;
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
	std::vector<std::pair<float, std::string>> meterData;
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "first"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "second"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "third"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "fourth"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "fifth"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "sixth"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "seventh"));
	meterData.push_back(std::pair<float, std::string>(float(rand()%100)*0.01f, "eighth"));

	auto meterSpacing = outerMargin * 0.5f;
	auto meterWidth = (visuArea.getWidth() - (meterData.size() + 1) * meterSpacing) / meterData.size();
    meterWidth = meterWidth > maxMeterWidth ? maxMeterWidth : meterWidth;
	auto meterMaxHeight = visuArea.getHeight() - 2 * meterSpacing;
	auto meterLeft = visuAreaOrigX + meterSpacing;
	Rectangle<int> meterRect;
	meterRect.setBottom(visuAreaOrigY);
	meterRect.setWidth(meterWidth);

	g.setColour(Colours::azure.darker());
	g.setFont(14.0f);
	for (const std::pair<float, std::string> &data : meterData)
	{
		auto meterHeight = meterMaxHeight * data.first;

		g.fillRect(Rectangle<int>(meterLeft, visuAreaOrigY - meterHeight, meterWidth, meterHeight));
		g.drawText(data.second, Rectangle<int>(meterLeft, visuAreaOrigY, meterWidth, outerMargin), Justification::centred, true);

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
    ignoreUnused(data);
}
