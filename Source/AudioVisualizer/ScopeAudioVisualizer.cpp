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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

ScopeAudioVisualizer::~ScopeAudioVisualizer()
{
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
	auto scopeRect = Rectangle<float>((getWidth() - scopeDiameter) * 0.5f, (getHeight() - scopeDiameter) * 0.5f, scopeDiameter, scopeDiameter);

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
    ignoreUnused(data);
}
