/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "MainHeader.h"
#include "AudioVisualizer/TwoDFieldAudioVisualizer.h"
#include "AudioVisualizer/MultiMeterAudioVisualizer.h"
#include "AudioVisualizer/ScopeAudioVisualizer.h"
#include "MainFooter.h"
#include "MainProcessor.h"

//==============================================================================
MainComponent::MainComponent()
{
    m_Header            = std::make_unique<MainHeader>();
	addAndMakeVisible(m_Header.get());
    m_AudioVisualizer   = std::make_unique<ScopeAudioVisualizer>();
	addAndMakeVisible(m_AudioVisualizer.get());
    m_Footer            = std::make_unique<MainFooter>();
	addAndMakeVisible(m_Footer.get());

    m_Processor         = std::make_unique<MainProcessor>();

	setSize(400, 600);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	auto panelMaxSize = 40.0f;
	auto isPortrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();
	
	FlexBox fb;
	if(isPortrait)
	{
		fb.flexDirection = FlexBox::Direction::column;
		fb.items.addArray({ FlexItem(*m_Header.get()).withFlex(1).withMaxHeight(panelMaxSize),
							FlexItem(*m_AudioVisualizer.get()).withFlex(4),
							FlexItem(*m_Footer.get()).withFlex(1).withMaxHeight(panelMaxSize) });
	}
	else
	{
		fb.flexDirection = FlexBox::Direction::row;
		fb.items.addArray({ FlexItem(*m_Header.get()).withFlex(1).withMaxWidth(panelMaxSize),
							FlexItem(*m_AudioVisualizer.get()).withFlex(4),
							FlexItem(*m_Footer.get()).withFlex(1).withMaxWidth(panelMaxSize) });
	}
	fb.performLayout(getLocalBounds().toFloat());
}
