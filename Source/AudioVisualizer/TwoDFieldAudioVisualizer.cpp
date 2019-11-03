/*
  ==============================================================================

    TwoDFieldAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TwoDFieldAudioVisualizer.h"

//==============================================================================
TwoDFieldAudioVisualizer::TwoDFieldAudioVisualizer()
    : AbstractAudioVisualizer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TwoDFieldAudioVisualizer::~TwoDFieldAudioVisualizer()
{
}

void TwoDFieldAudioVisualizer::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // calculate what we need for our center circle
    auto width = getWidth();
    auto height = getHeight();
    auto outerMargin = 40;
    auto visuAreaWidth = width - 2 * outerMargin;
    auto visuAreaHeight = height - 2 * outerMargin;

    Rectangle<int> visuArea(outerMargin, outerMargin, visuAreaWidth, visuAreaHeight);

    // fill our visualization area background
    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    g.fillRect(visuArea);

    auto visuAreaOrigX = float(outerMargin);
    auto visuAreaOrigY = float(outerMargin + visuAreaHeight);

    // draw a simple frame
    g.setColour(Colours::white);
    g.drawLine(Line<float>(visuAreaOrigX, visuAreaOrigY, visuAreaOrigX + visuAreaWidth, visuAreaOrigY));
    g.drawLine(Line<float>(visuAreaOrigX, visuAreaOrigY - visuAreaHeight, visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight));
    g.drawLine(Line<float>(visuAreaOrigX, visuAreaOrigY, visuAreaOrigX, visuAreaOrigY - visuAreaHeight));
    g.drawLine(Line<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY, visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight));
    
    // draw dashed field dimension indication lines
    float dparam[]{ 4.0f, 5.0f };
    g.drawDashedLine(Line<float>(visuAreaOrigX, visuAreaOrigY, visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight), dparam, 2);
    g.drawDashedLine(Line<float>(visuAreaOrigX, visuAreaOrigY - visuAreaHeight, visuAreaOrigX + visuAreaWidth, visuAreaOrigY), dparam, 2);
    g.drawDashedLine(Line<float>(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - 0.5f*visuAreaHeight, visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - visuAreaHeight), dparam, 2);
    
    // draw level indication lines
    float levelL = float(rand()%100)*0.01f;
    float levelC = float(rand()%100)*0.01f;
    float levelR = float(rand()%100)*0.01f;
    float levelRS = float(rand()%100)*0.01f;
    float levelLS = float(rand()%100)*0.01f;
    
    Point<float> levelOrig(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - 0.5f*visuAreaHeight);
    Point<float> leftMax = levelOrig - Point<float>(visuAreaOrigX, visuAreaOrigY - visuAreaHeight);
    Point<float> centerMax = levelOrig - Point<float>(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    Point<float> rightMax = levelOrig - Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    Point<float> rightSurroundMax = levelOrig - Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY);
    Point<float> leftSurroundMax = levelOrig - Point<float>(visuAreaOrigX, visuAreaOrigY);
    
    g.setColour(Colours::azure.darker());
    Path path;
    path.startNewSubPath(levelOrig - leftMax*levelL);
    path.lineTo(levelOrig - centerMax*levelC);
    path.lineTo(levelOrig - rightMax*levelR);
    path.lineTo(levelOrig - rightSurroundMax*levelRS);
    path.lineTo(levelOrig - leftSurroundMax*levelLS);
    path.lineTo(levelOrig - leftMax*levelL);
    g.strokePath(path, PathStrokeType(2));
}

void TwoDFieldAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType TwoDFieldAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::TwoDField;
}
