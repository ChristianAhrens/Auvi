/*
  ==============================================================================

    ScopeAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:27am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "ScopeAudioVisualizer.h"

#include "../utils.hpp"

namespace Auvi
{

//==============================================================================
ScopeAudioVisualizer::ScopeAudioVisualizer()
    : AbstractAudioVisualizer()
{
    showConfigButton(true);

    m_channelMapping = {
        {AudioVisualizerConfigBase::MappingKey::X, m_channelX},
        {AudioVisualizerConfigBase::MappingKey::Y, m_channelY},
    };
    m_usesValuesInDB = false;

    m_scopeTail.resize(m_scopeTailLength);
}

ScopeAudioVisualizer::~ScopeAudioVisualizer()
{
}

unsigned long ScopeAudioVisualizer::GetNextScopeTailPos()
{
    if (m_scopeTailPos < m_scopeTailLength - 1)
        m_scopeTailPos++;
    else if (m_scopeTailPos == m_scopeTailLength - 1)
        m_scopeTailPos = 0;
    else
        m_scopeTailPos = 0;

    return m_scopeTailPos;
}

juce::Point<float> ScopeAudioVisualizer::MapValToRect(float x, float y, Rectangle<float> rectF)
{
    float angle = (x != 0) ? atanf(fabs(y) / fabs(x)) * (180 / float_Pi) : 0;
    float corr = (1 - (fabs(angle - 45.0f) / 45.0f)) * 0.70710678f; // 0.70710678f=2/sqrt(2*2+2*2)

    float pointX = corr * (0.5f * rectF.getWidth()) * x;
    float pointY = corr * (0.5f * rectF.getHeight()) * y;

    return juce::Point<float>(rectF.getCentre() + juce::Point<float>(pointX, pointY));
}

void ScopeAudioVisualizer::paint(Graphics& g)
{
    AbstractAudioVisualizer::paint(g);

    // fill the background as starting point
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // calculate what we need for our center circle
    auto outerMargin = 20;
    auto legendMarkerSize = 20;
    auto width = getWidth() - 2 * outerMargin;
    auto height = getHeight() - 2 * outerMargin;
    auto scopeDiameter = width < height ? width : height;
    auto scopeRect = Rectangle<float>(float(getWidth() - scopeDiameter) * 0.5f, float(getHeight() - scopeDiameter) * 0.5f, scopeDiameter, scopeDiameter);

    // fill our visualization area background
    g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    g.fillEllipse(scopeRect);

    // outer and inner circle
    g.setColour(Colours::white);
    g.drawEllipse(scopeRect, 4); // outer circle
    g.drawEllipse(scopeRect.reduced(0.25f * scopeDiameter), 2); // inner circle

    // crosshairs
    g.drawLine(Line<float>((getWidth() - scopeDiameter) * 0.5f - outerMargin, getHeight() * 0.5f,
        getWidth() * 0.5f + scopeDiameter * 0.5f + outerMargin, getHeight() * 0.5f), 2); // horizontal crosshair
    g.drawLine(Line<float>(getWidth() * 0.5f, (getHeight() - scopeDiameter) * 0.5f - outerMargin,
        getWidth() * 0.5f, getHeight() * 0.5f + scopeDiameter * 0.5f + outerMargin), 2); // vertical crosshair

    // crosshair legend
    g.drawText("X", Rectangle<float>((getWidth() - scopeDiameter) * 0.5f - outerMargin, getHeight() * 0.5f, float(outerMargin), float(outerMargin)), Justification::centred, true);
    g.drawText("Y", Rectangle<float>(getWidth() * 0.5f, (getHeight() - scopeDiameter) * 0.5f - outerMargin, float(outerMargin), float(outerMargin)), Justification::centred, true);

    // horizontal legend marker lines
    g.drawLine(Line<float>(scopeRect.getX() + 0.125f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
        scopeRect.getX() + 0.125f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));
    g.drawLine(Line<float>(scopeRect.getX() + 0.375f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
        scopeRect.getX() + 0.375f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));
    g.drawLine(Line<float>(scopeRect.getX() + 0.625f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
        scopeRect.getX() + 0.625f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));
    g.drawLine(Line<float>(scopeRect.getX() + 0.875f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
        scopeRect.getX() + 0.875f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));

    // vertical legend marker lines
    g.drawLine(Line<float>(scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.125f * scopeDiameter,
        scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.125f * scopeDiameter));
    g.drawLine(Line<float>(scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.375f * scopeDiameter,
        scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.375f * scopeDiameter));
    g.drawLine(Line<float>(scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.625f * scopeDiameter,
        scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.625f * scopeDiameter));
    g.drawLine(Line<float>(scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.875f * scopeDiameter,
        scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.875f * scopeDiameter));

    // legend values
    g.setFont(12.0f);
    g.setColour(Colours::grey);
    g.drawText("0", Rectangle<float>(scopeRect.getX() + 0.5f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter, float(outerMargin), float(outerMargin)), Justification::centred, true);
    g.drawText("0.5", Rectangle<float>(scopeRect.getX() + 0.25f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter, float(outerMargin), float(outerMargin)), Justification::centred, true);
    g.drawText("1", Rectangle<float>(scopeRect.getX(), scopeRect.getY() + 0.5f * scopeDiameter, float(outerMargin), float(outerMargin)), Justification::centred, true);
    g.drawText("0.5", Rectangle<float>(scopeRect.getX() + 0.5f * scopeDiameter, scopeRect.getY() + 0.25f * scopeDiameter, float(outerMargin), float(outerMargin)), Justification::centred, true);
    g.drawText("1", Rectangle<float>(scopeRect.getX() + 0.5f * scopeDiameter, scopeRect.getY(), float(outerMargin), float(outerMargin)), Justification::centred, true);

    //// scope curve
    //juce::Point<float> newPoint = MapValToRect(m_scopeTail.at(m_scopeTailPos).first, m_scopeTail.at(m_scopeTailPos).second, scopeRect);
    //Path path;
    //path.startNewSubPath(newPoint);
    //unsigned long pos = m_scopeTailPos + 1;
    //for (unsigned long i = 0; i < m_scopeTailLength; ++i)
    //{
    //    if (pos < m_scopeTailLength - 1)
    //        pos++;
    //    else if (pos == m_scopeTailLength - 1)
    //        pos = 0;
    //    else
    //        pos = 0;
    //
    //    newPoint = MapValToRect(m_scopeTail.at(pos).first, m_scopeTail.at(pos).second, scopeRect);
    //    path.lineTo(newPoint);
    //}
    //g.setColour(Colours::forestgreen);
    //g.strokePath(path, PathStrokeType(1));

    // scope pointcloud
    g.setColour(Colours::forestgreen);
    juce::RectangleList<float> rectangles;
    unsigned long pos = m_scopeTailPos;
    juce::Point<float> newPoint;
    for (unsigned long i = 0; i < m_scopeTailLength; ++i)
    {
        newPoint = MapValToRect(m_scopeTail.at(pos).first, m_scopeTail.at(pos).second, scopeRect);
        rectangles.add(newPoint.getX(), newPoint.getY(), m_dotDiameter, m_dotDiameter);

        if (pos < m_scopeTailLength - 1)
            pos++;
        else if (pos == m_scopeTailLength - 1)
            pos = 0;
        else
            pos = 0;
    }
    g.fillRectList(rectangles);
}

void ScopeAudioVisualizer::resized()
{
    AbstractAudioVisualizer::resized();
}

AbstractAudioVisualizer::VisuType ScopeAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Scope;
}

void ScopeAudioVisualizer::processChangedChannelMapping()
{
    m_channelX = m_channelMapping.at(AudioVisualizerConfigBase::MappingKey::X);
    m_channelY = m_channelMapping.at(AudioVisualizerConfigBase::MappingKey::Y);
}

void ScopeAudioVisualizer::processingDataChanged(AbstractProcessorData* data)
{
    if (!data)
        return;

    switch (data->GetDataType())
    {
    case AbstractProcessorData::AudioSignal:
    {
        ProcessorAudioSignalData* sd = static_cast<ProcessorAudioSignalData*>(data);
        if (sd->GetChannelCount() >= m_channelX && sd->GetChannelCount() >= m_channelY)
        {
            auto block = juce::dsp::AudioBlock<float>(*static_cast<AudioBuffer<float>*>(sd));

            auto tailPos = GetNextScopeTailPos();

            auto minmaxX = block.getSingleChannelBlock(m_channelX-1).findMinAndMax();
            auto maxX = std::fabs(minmaxX.getStart()) > std::fabs(minmaxX.getEnd()) ? minmaxX.getStart() : minmaxX.getEnd();
            auto minmaxY = block.getSingleChannelBlock(m_channelY-1).findMinAndMax();
            auto maxY = std::fabs(minmaxY.getStart()) > std::fabs(minmaxY.getEnd()) ? minmaxY.getStart() : minmaxY.getEnd();

            if (m_usesValuesInDB)
            {
                auto mindB = static_cast<float>(Auvi::utils::getGlobalMindB());
                auto maxdB = static_cast<float>(Auvi::utils::getGlobalMaxdB());
                auto maxXdB = jlimit(mindB, maxdB, Decibels::gainToDecibels(maxX));
                maxXdB = jmap(maxXdB, mindB, maxdB, m_min, m_max);
                auto maxYdB = jlimit(mindB, maxdB, Decibels::gainToDecibels(maxY));
                maxYdB = jmap(maxYdB, mindB, maxdB, m_min, m_max);
                m_scopeTail[tailPos] = std::make_pair(maxXdB, maxYdB);
            }
            else
                m_scopeTail[tailPos] = std::make_pair(maxX, maxY);

            notifyChanges();
        }
    }
    break;
    case AbstractProcessorData::Level:
    case AbstractProcessorData::Spectrum:
    case AbstractProcessorData::Invalid:
    default:
        break;
    }
}

}
