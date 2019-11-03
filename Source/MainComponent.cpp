/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "MainHeader.h"
#include "MainFooter.h"
#include "MainProcessor.h"
#include "AudioVisualizer/TwoDFieldAudioVisualizer.h"
#include "AudioVisualizer/MultiMeterAudioVisualizer.h"
#include "AudioVisualizer/ScopeAudioVisualizer.h"
#include "AudioVisualizer/RtaAudioVisualizer.h"
#include "AudioVisualizer/WaterfallAudioVisualizer.h"
#include "AudioVisualizer/WaveformAudioVisualizer.h"

//==============================================================================
MainComponent::MainComponent()
{
    m_Header            = std::make_unique<MainHeader>();
	addAndMakeVisible(m_Header.get());
    m_Footer            = std::make_unique<MainFooter>();
	addAndMakeVisible(m_Footer.get());

    m_Processor         = std::make_unique<MainProcessor>();
    
    updateVisuType(AbstractAudioVisualizer::VisuType::Scope);

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

void MainComponent::updateVisuType(AbstractAudioVisualizer::VisuType type)
{
    m_AudioVisualizer.reset();
    
    switch(type)
    {
        case AbstractAudioVisualizer::VisuType::MultiMeter:
            m_AudioVisualizer = std::make_unique<MultiMeterAudioVisualizer>();
            break;
        case AbstractAudioVisualizer::VisuType::TwoDField:
            m_AudioVisualizer = std::make_unique<TwoDFieldAudioVisualizer>();
            break;
        case AbstractAudioVisualizer::VisuType::Rta:
            m_AudioVisualizer = std::make_unique<RtaAudioVisualizer>();
            break;
        case AbstractAudioVisualizer::VisuType::Waterfall:
            m_AudioVisualizer = std::make_unique<WaterfallAudioVisualizer>();
            break;
        case AbstractAudioVisualizer::VisuType::Waveform:
            m_AudioVisualizer = std::make_unique<WaveformAudioVisualizer>();
            break;
        default:
            m_AudioVisualizer = std::make_unique<ScopeAudioVisualizer>();
            break;
    }
    
    addAndMakeVisible(m_AudioVisualizer.get());
    
    resized();
}
