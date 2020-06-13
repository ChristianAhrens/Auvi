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
    setConfigFeatures(AudioVisualizerConfigBase::ConfigFeatures::ChannelMapping | AudioVisualizerConfigBase::ConfigFeatures::UseValuesInDBToogle);
    setChannelMapping({
        {AudioVisualizerConfigBase::MappingKey::L, m_channelL},
        {AudioVisualizerConfigBase::MappingKey::C, m_channelC},
        {AudioVisualizerConfigBase::MappingKey::R, m_channelR},
        {AudioVisualizerConfigBase::MappingKey::LS, m_channelLS},
        {AudioVisualizerConfigBase::MappingKey::RS, m_channelRS},
        });
    setUsesValuesInDB(true);
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
    String rangeText;
    if (getUsesValuesInDB())
        rangeText = String(Auvi::utils::getGlobalMindB()) + " ... " + String(Auvi::utils::getGlobalMaxdB()) + " dBFS";
    else
        rangeText = "0 ... 1";
    g.drawText(rangeText, Rectangle<float>(visuAreaOrigX + visuAreaWidth - 110.0f, visuAreaOrigY - visuAreaHeight - 5.0f, 110.0f, float(outerMargin)), Justification::centred, true);

    // draw level indication lines
    juce::Point<float> levelOrig(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - 0.5f*visuAreaHeight);
    juce::Point<float> leftMax = levelOrig - juce::Point<float>(visuAreaOrigX, visuAreaOrigY - visuAreaHeight);
    juce::Point<float> centerMax = levelOrig - juce::Point<float>(visuAreaOrigX + 0.5f*visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    juce::Point<float> rightMax = levelOrig - juce::Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY - visuAreaHeight);
    juce::Point<float> rightSurroundMax = levelOrig - juce::Point<float>(visuAreaOrigX + visuAreaWidth, visuAreaOrigY);
    juce::Point<float> leftSurroundMax = levelOrig - juce::Point<float>(visuAreaOrigX, visuAreaOrigY);

    // hold values
    float holdLevelL  {0};
    float holdLevelC  {0};
    float holdLevelR  {0};
    float holdLevelLS {0};
    float holdLevelRS {0};
    if (getUsesValuesInDB())
    {
        holdLevelL = m_levelData.GetLevel(m_channelL).GetFactorHOLDdB();
        holdLevelC = m_levelData.GetLevel(m_channelC).GetFactorHOLDdB();
        holdLevelR = m_levelData.GetLevel(m_channelR).GetFactorHOLDdB();
        holdLevelLS = m_levelData.GetLevel(m_channelLS).GetFactorHOLDdB();
        holdLevelRS = m_levelData.GetLevel(m_channelRS).GetFactorHOLDdB();
    }
    else
    {
        holdLevelL = m_levelData.GetLevel(m_channelL).hold;
        holdLevelC = m_levelData.GetLevel(m_channelC).hold;
        holdLevelR = m_levelData.GetLevel(m_channelR).hold;
        holdLevelLS = m_levelData.GetLevel(m_channelLS).hold;
        holdLevelRS = m_levelData.GetLevel(m_channelRS).hold;
    }

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
    float peakLevelL  {0};
    float peakLevelC  {0};
    float peakLevelR  {0};
    float peakLevelLS {0};
    float peakLevelRS {0};
    if (getUsesValuesInDB())
    {
        peakLevelL = m_levelData.GetLevel(m_channelL).GetFactorPEAKdB();
        peakLevelC = m_levelData.GetLevel(m_channelC).GetFactorPEAKdB();
        peakLevelR = m_levelData.GetLevel(m_channelR).GetFactorPEAKdB();
        peakLevelLS = m_levelData.GetLevel(m_channelLS).GetFactorPEAKdB();
        peakLevelRS = m_levelData.GetLevel(m_channelRS).GetFactorPEAKdB();
    }
    else
    {
        peakLevelL = m_levelData.GetLevel(m_channelL).peak;
        peakLevelC = m_levelData.GetLevel(m_channelC).peak;
        peakLevelR = m_levelData.GetLevel(m_channelR).peak;
        peakLevelLS = m_levelData.GetLevel(m_channelLS).peak;
        peakLevelRS = m_levelData.GetLevel(m_channelRS).peak;
    }

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
    float rmsLevelL  {0};
    float rmsLevelC  {0};
    float rmsLevelR  {0};
    float rmsLevelLS {0};
    float rmsLevelRS {0};
    if (getUsesValuesInDB())
    {
        rmsLevelL = m_levelData.GetLevel(m_channelL).GetFactorRMSdB();
        rmsLevelC = m_levelData.GetLevel(m_channelC).GetFactorRMSdB();
        rmsLevelR = m_levelData.GetLevel(m_channelR).GetFactorRMSdB();
        rmsLevelLS = m_levelData.GetLevel(m_channelLS).GetFactorRMSdB();
        rmsLevelRS = m_levelData.GetLevel(m_channelRS).GetFactorRMSdB();
    }
    else
    {
        rmsLevelL = m_levelData.GetLevel(m_channelL).rms;
        rmsLevelC = m_levelData.GetLevel(m_channelC).rms;
        rmsLevelR = m_levelData.GetLevel(m_channelR).rms;
        rmsLevelLS = m_levelData.GetLevel(m_channelLS).rms;
        rmsLevelRS = m_levelData.GetLevel(m_channelRS).rms;
    }

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
    if(getChannelMapping().count(AudioVisualizerConfigBase::MappingKey::L))
        m_channelL = getChannelMapping().at(AudioVisualizerConfigBase::MappingKey::L);
    if(getChannelMapping().count(AudioVisualizerConfigBase::MappingKey::C))
        m_channelC = getChannelMapping().at(AudioVisualizerConfigBase::MappingKey::C);
    if(getChannelMapping().count(AudioVisualizerConfigBase::MappingKey::R))
        m_channelR = getChannelMapping().at(AudioVisualizerConfigBase::MappingKey::R);
    if(getChannelMapping().count(AudioVisualizerConfigBase::MappingKey::LS))
        m_channelLS = getChannelMapping().at(AudioVisualizerConfigBase::MappingKey::LS);
    if(getChannelMapping().count(AudioVisualizerConfigBase::MappingKey::RS))
        m_channelRS = getChannelMapping().at(AudioVisualizerConfigBase::MappingKey::RS);
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
