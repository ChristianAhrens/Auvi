/*
  ==============================================================================

    ScopeAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:27am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ScopeAudioVisualizer.h"

//==============================================================================
ScopeAudioVisualizer::ScopeAudioVisualizer()
    : AbstractAudioVisualizer()
{
    m_channelX = 1;
    m_channelY = 5;
    m_scopeTailLength = 50;
    m_scopeTailPos = 0;
    m_scopeTailX.resize(m_scopeTailLength);
    m_scopeTailY.resize(m_scopeTailLength);
}

ScopeAudioVisualizer::~ScopeAudioVisualizer()
{
}

unsigned long ScopeAudioVisualizer::GetNextScopeTailPos()
{
    if(m_scopeTailPos < m_scopeTailLength-1)
        m_scopeTailPos++;
    else if(m_scopeTailPos == m_scopeTailLength-1)
        m_scopeTailPos = 0;
    else
        m_scopeTailPos = 0;
    
    return m_scopeTailPos;
}

Point<float> ScopeAudioVisualizer::MapValToRect(float x, float y, Rectangle<float> rectF)
{
    return Point<float>(rectF.getX()+(rectF.getWidth()*x), rectF.getY()+(rectF.getHeight()*y));
}

void ScopeAudioVisualizer::paint (Graphics& g)
{
	// fill the background as starting point
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

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
    g.setColour (Colours::white);
	g.drawEllipse(scopeRect, 4); // outer circle
	g.drawEllipse(scopeRect.reduced(0.25f * scopeDiameter), 2); // inner circle

	// crosshairs
	g.drawLine(Line<float>((getWidth() - scopeDiameter) * 0.5f - outerMargin, getHeight() * 0.5f,
		getWidth() * 0.5f + scopeDiameter * 0.5f + outerMargin, getHeight() * 0.5f), 2); // horizontal crosshair
	g.drawLine(Line<float>(getWidth() * 0.5f, (getHeight() - scopeDiameter) * 0.5f - outerMargin,
		getWidth() * 0.5f, getHeight() * 0.5f + scopeDiameter * 0.5f + outerMargin), 2); // vertical crosshair

	// horizontal legend marker lines
	g.drawLine(Line<float>(	scopeRect.getX() + 0.125f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
							scopeRect.getX() + 0.125f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));
	g.drawLine(Line<float>(	scopeRect.getX() + 0.375f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
							scopeRect.getX() + 0.375f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));
	g.drawLine(Line<float>(	scopeRect.getX() + 0.625f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
							scopeRect.getX() + 0.625f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));
	g.drawLine(Line<float>(	scopeRect.getX() + 0.875f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize,
							scopeRect.getX() + 0.875f * scopeDiameter, scopeRect.getY() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize));

	// vertical legend marker lines
	g.drawLine(Line<float>(	scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.125f * scopeDiameter,
							scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.125f * scopeDiameter));
	g.drawLine(Line<float>(	scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.375f * scopeDiameter,
							scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.375f * scopeDiameter));
	g.drawLine(Line<float>(	scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.625f * scopeDiameter,
							scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.625f * scopeDiameter));
	g.drawLine(Line<float>(	scopeRect.getX() + 0.5f * scopeDiameter + 0.5 * legendMarkerSize, scopeRect.getY() + 0.875f * scopeDiameter,
							scopeRect.getX() + 0.5f * scopeDiameter - 0.5 * legendMarkerSize, scopeRect.getY() + 0.875f * scopeDiameter));
    
    // scope curve
    Point<float> newPeakPoint = MapValToRect(m_scopeTailX.at(m_scopeTailPos).peak, m_scopeTailY.at(m_scopeTailPos).peak, scopeRect);
    Point<float> newRmsPoint = MapValToRect(m_scopeTailX.at(m_scopeTailPos).rms, m_scopeTailY.at(m_scopeTailPos).rms, scopeRect);
    Path peakPath, rmsPath;
    peakPath.startNewSubPath(newPeakPoint);
    rmsPath.startNewSubPath(newRmsPoint);
    unsigned long pos = m_scopeTailPos+1;
    for(int i = 0; i < m_scopeTailLength; ++i)
    {
        if(pos < m_scopeTailLength-1)
            pos++;
        else if(pos == m_scopeTailLength-1)
            pos = 0;
        else
            pos = 0;
        
        newPeakPoint = MapValToRect(m_scopeTailX.at(pos).peak, m_scopeTailY.at(pos).peak, scopeRect);
        peakPath.lineTo(newPeakPoint);
        newRmsPoint = MapValToRect(m_scopeTailX.at(pos).rms, m_scopeTailY.at(pos).rms, scopeRect);
        rmsPath.lineTo(newRmsPoint);
    }
    g.setColour(Colours::forestgreen.darker());
    g.strokePath(peakPath, PathStrokeType(3));
    g.setColour(Colours::forestgreen);
    g.strokePath(rmsPath, PathStrokeType(2));
}

void ScopeAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType ScopeAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Scope;
}

void ScopeAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    if(!data)
        return;
    
    switch(data->GetDataType())
    {
        case AbstractProcessorData::Level:
        {
            ProcessorLevelData* ld = static_cast<ProcessorLevelData*>(data);
            jassert(ld->GetChannelCount()>1);
            if(ld->GetChannelCount()>1)
            {
                unsigned long iter = GetNextScopeTailPos();
                m_scopeTailX[iter] = ld->GetLevel(m_channelX);
                m_scopeTailY[iter] = ld->GetLevel(m_channelY);
            }
            repaint();
            break;
        }
        case AbstractProcessorData::Spectrum:
            break;
        case AbstractProcessorData::Invalid:
        default:
            break;
    }
}
