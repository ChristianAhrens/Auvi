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
    m_channelL = 1;
    m_channelC = 2;
    m_channelR = 3;
    m_channelLS = 5;
    m_channelRS = 4;
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
    auto outerMargin = 30;
    
    auto visuAreaWidth = (width < height ? width : height) - 2 * outerMargin;
    auto visuAreaHeight = (width < height ? width : height) - 2 * outerMargin;
    
    auto visuAreaOrigX = float(0.5f * (width - visuAreaWidth));
    auto visuAreaOrigY = height - float(0.5f * (height - visuAreaHeight));

    Rectangle<int> visuArea(visuAreaOrigX, visuAreaOrigY - visuAreaHeight, visuAreaWidth, visuAreaHeight);

    // fill our visualization area background
    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    g.fillRect(visuArea);

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
    Point<float> levelOrig(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - 0.5f*visuAreaHeight);
    Point<float> leftMax = levelOrig - Point<float>(visuAreaOrigX, visuAreaOrigY - visuAreaHeight);
    Point<float> centerMax = levelOrig - Point<float>(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    Point<float> rightMax = levelOrig - Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    Point<float> rightSurroundMax = levelOrig - Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY);
    Point<float> leftSurroundMax = levelOrig - Point<float>(visuAreaOrigX, visuAreaOrigY);
    
    float peakLevelL = m_levelData.GetLevel(m_channelL).peak;
    float peakLevelC = m_levelData.GetLevel(m_channelC).peak;
    float peakLevelR = m_levelData.GetLevel(m_channelR).peak;
    float peakLevelLS = m_levelData.GetLevel(m_channelLS).peak;
    float peakLevelRS = m_levelData.GetLevel(m_channelRS).peak;

    g.setColour(Colours::forestgreen.darker());
    Path peakPath;
    peakPath.startNewSubPath(levelOrig - leftMax * peakLevelL);
    peakPath.lineTo(levelOrig - centerMax * peakLevelC);
    peakPath.lineTo(levelOrig - rightMax * peakLevelR);
    peakPath.lineTo(levelOrig - rightSurroundMax * peakLevelRS);
    peakPath.lineTo(levelOrig - leftSurroundMax * peakLevelLS);
    peakPath.lineTo(levelOrig - leftMax * peakLevelL);
    g.strokePath(peakPath, PathStrokeType(3));
    
    float rmsLevelL = m_levelData.GetLevel(m_channelL).rms;
    float rmsLevelC = m_levelData.GetLevel(m_channelC).rms;
    float rmsLevelR = m_levelData.GetLevel(m_channelR).rms;
    float rmsLevelLS = m_levelData.GetLevel(m_channelLS).rms;
    float rmsLevelRS = m_levelData.GetLevel(m_channelRS).rms;

    g.setColour(Colours::forestgreen);
    Path rmsPath;
    rmsPath.startNewSubPath(levelOrig - leftMax * rmsLevelL);
    rmsPath.lineTo(levelOrig - centerMax * rmsLevelC);
    rmsPath.lineTo(levelOrig - rightMax * rmsLevelR);
    rmsPath.lineTo(levelOrig - rightSurroundMax * rmsLevelRS);
    rmsPath.lineTo(levelOrig - leftSurroundMax * rmsLevelLS);
    rmsPath.lineTo(levelOrig - leftMax * rmsLevelL);
    g.strokePath(rmsPath, PathStrokeType(2));
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

void TwoDFieldAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
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
