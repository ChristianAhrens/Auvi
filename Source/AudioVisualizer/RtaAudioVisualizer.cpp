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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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

    // draw a simple baseline
    g.setColour(Colours::white);
    g.drawLine(Line<float>(visuAreaOrigX, visuAreaOrigY, visuAreaOrigX + visuAreaWidth, visuAreaOrigY));

    // draw dummy curve
    g.setColour(Colours::forestgreen);
    
    std::vector<float> plotPoints;
    for(int i=0; i<50; ++i)
    {
        plotPoints.push_back(float(rand()%100)*0.01f);
    }
    
    float newPointX = visuAreaOrigX;
    float newPointY = visuAreaOrigY - plotPoints.front()*visuAreaHeight;
    float plotStepWidth = plotPoints.size()>0 ? float(visuAreaWidth)/float(plotPoints.size()-1) : 1;
    
    Path path;
    path.startNewSubPath(Point<float>(newPointX, newPointY));
    for(int i = 1; i < plotPoints.size(); ++i)
    {
        newPointX += plotStepWidth;
        newPointY = visuAreaOrigY - plotPoints.at(i)*visuAreaHeight;
        
        path.lineTo(Point<float>(newPointX, newPointY));
    }
    g.strokePath(path, PathStrokeType(3));
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
    ignoreUnused(data);
}
