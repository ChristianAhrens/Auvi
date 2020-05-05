/*
  ==============================================================================

    TwoDFieldAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "TwoDFieldAudioVisualizer.h"

#include "../utils.hpp"

namespace Auvi
{

//==============================================================================
TwoDFieldAudioVisualizer::TwoDFieldAudioVisualizer()
    : AbstractAudioVisualizer()
{
    showConfigButton(true);

    m_channelL = 1;
    m_channelC = 2;
    m_channelR = 3;
    m_channelLS = 5;
    m_channelRS = 4;

    m_channelMapping = { {"Left", m_channelL}, {"Center", m_channelC}, {"Right", m_channelR}, {"Left Surround", m_channelLS}, {"Right Surround", m_channelRS}, };
}

TwoDFieldAudioVisualizer::~TwoDFieldAudioVisualizer()
{
}

void TwoDFieldAudioVisualizer::paint (Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // calculate what we need for our center circle
    auto width = getWidth();
    auto height = getHeight();
    auto outerMargin = 30.0f;
    
    auto visuAreaWidth = static_cast<float>(width < height ? width : height) - 2 * outerMargin;
    auto visuAreaHeight = static_cast<float>(width < height ? width : height) - 2 * outerMargin;
    
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

    // draw L C R LS RS legend
    g.drawText("L", Rectangle<float>(visuAreaOrigX - 20.0f, visuAreaOrigY - visuAreaHeight - float(outerMargin), 40.0f, float(outerMargin)), Justification::centred, true);
    g.drawText("C", Rectangle<float>(visuAreaOrigX + 0.5f*visuAreaWidth - 20.0f, visuAreaOrigY - visuAreaHeight - float(outerMargin), 40.0f, float(outerMargin)), Justification::centred, true);
    g.drawText("R", Rectangle<float>(visuAreaOrigX + visuAreaWidth - 20.0f, visuAreaOrigY - visuAreaHeight - float(outerMargin), 40.0f, float(outerMargin)), Justification::centred, true);
    g.drawText("LS", Rectangle<float>(visuAreaOrigX - 20.0f, visuAreaOrigY, 40.0f, float(outerMargin)), Justification::centred, true);
    g.drawText("RS", Rectangle<float>(visuAreaOrigX + visuAreaWidth - 20.0f, visuAreaOrigY, 40.0f, float(outerMargin)), Justification::centred, true);

    // draw dBFS
    g.setFont(12.0f);
    g.setColour(Colours::grey);
    g.drawText(String(Auvi::utils::getGlobalMindB()) + " ... " + String(Auvi::utils::getGlobalMaxdB()) + " dBFS", Rectangle<float>(visuAreaOrigX + visuAreaWidth - 110.0f, visuAreaOrigY - visuAreaHeight - 5.0f, 110.0f, float(outerMargin)), Justification::centred, true);

    // draw level indication lines
    juce::Point<float> levelOrig(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - 0.5f*visuAreaHeight);
    juce::Point<float> leftMax = levelOrig - juce::Point<float>(visuAreaOrigX, visuAreaOrigY - visuAreaHeight);
    juce::Point<float> centerMax = levelOrig - juce::Point<float>(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    juce::Point<float> rightMax = levelOrig - juce::Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    juce::Point<float> rightSurroundMax = levelOrig - juce::Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY);
    juce::Point<float> leftSurroundMax = levelOrig - juce::Point<float>(visuAreaOrigX, visuAreaOrigY);

    // hold values
    float holdLevelL = m_levelData.GetLevel(m_channelL).GetFactorHOLDdB();
    float holdLevelC = m_levelData.GetLevel(m_channelC).GetFactorHOLDdB();
    float holdLevelR = m_levelData.GetLevel(m_channelR).GetFactorHOLDdB();
    float holdLevelLS = m_levelData.GetLevel(m_channelLS).GetFactorHOLDdB();
    float holdLevelRS = m_levelData.GetLevel(m_channelRS).GetFactorHOLDdB();

    g.setColour(Colours::grey);
    Path holdPath;
    holdPath.startNewSubPath(levelOrig - leftMax * holdLevelL);
    holdPath.lineTo(levelOrig - centerMax * holdLevelC);
    holdPath.lineTo(levelOrig - rightMax * holdLevelR);
    holdPath.lineTo(levelOrig - rightSurroundMax * holdLevelRS);
    holdPath.lineTo(levelOrig - leftSurroundMax * holdLevelLS);
    holdPath.lineTo(levelOrig - leftMax * holdLevelL);
    g.strokePath(holdPath, PathStrokeType(1));

    // peak values
    float peakLevelL = m_levelData.GetLevel(m_channelL).GetFactorPEAKdB();
    float peakLevelC = m_levelData.GetLevel(m_channelC).GetFactorPEAKdB();
    float peakLevelR = m_levelData.GetLevel(m_channelR).GetFactorPEAKdB();
    float peakLevelLS = m_levelData.GetLevel(m_channelLS).GetFactorPEAKdB();
    float peakLevelRS = m_levelData.GetLevel(m_channelRS).GetFactorPEAKdB();

    g.setColour(Colours::forestgreen.darker());
    Path peakPath;
    peakPath.startNewSubPath(levelOrig - leftMax * peakLevelL);
    peakPath.lineTo(levelOrig - centerMax * peakLevelC);
    peakPath.lineTo(levelOrig - rightMax * peakLevelR);
    peakPath.lineTo(levelOrig - rightSurroundMax * peakLevelRS);
    peakPath.lineTo(levelOrig - leftSurroundMax * peakLevelLS);
    peakPath.lineTo(levelOrig - leftMax * peakLevelL);
    g.strokePath(peakPath, PathStrokeType(3));

    // rms values
    float rmsLevelL = m_levelData.GetLevel(m_channelL).GetFactorRMSdB();
    float rmsLevelC = m_levelData.GetLevel(m_channelC).GetFactorRMSdB();
    float rmsLevelR = m_levelData.GetLevel(m_channelR).GetFactorRMSdB();
    float rmsLevelLS = m_levelData.GetLevel(m_channelLS).GetFactorRMSdB();
    float rmsLevelRS = m_levelData.GetLevel(m_channelRS).GetFactorRMSdB();

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
    AbstractAudioVisualizer::resized();
}

AbstractAudioVisualizer::VisuType TwoDFieldAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::TwoDField;
}

void TwoDFieldAudioVisualizer::processChangedChannelMapping()
{
    m_channelL = m_channelMapping.at("Left");
    m_channelC = m_channelMapping.at("Center");
    m_channelR = m_channelMapping.at("Right");
    m_channelLS = m_channelMapping.at("Left Surround");
    m_channelRS = m_channelMapping.at("Right Surround");
}

void TwoDFieldAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if(!data)
        return;
    
    switch(data->GetDataType())
    {
        case AbstractProcessorData::Level:
            m_levelData = *(static_cast<ProcessorLevelData*>(data));
            notifyChanges();
            break;
        case AbstractProcessorData::AudioSignal:
        case AbstractProcessorData::Spectrum:
        case AbstractProcessorData::Invalid:
        default:
            break;
    }
}

}
