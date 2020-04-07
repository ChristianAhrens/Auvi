/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "utils.hpp"
#include "Header.h"
#include "Footer.h"
#include "Processor/Processor.h"

//==============================================================================
MainComponent::MainComponent()
{
	m_audioConfig = nullptr;

    m_header            = std::make_unique<Header>();
	addAndMakeVisible(m_header.get());
    m_body              = std::make_unique<Body>();
    addAndMakeVisible(m_body.get());
    m_footer            = std::make_unique<Footer>();
	addAndMakeVisible(m_footer.get());
    
    m_deviceManager.initialiseWithDefaultDevices(2, 0);
#if JUCE_IOS
    auto currentSetup = m_deviceManager.getAudioDeviceSetup();
    currentSetup.bufferSize = 512; // temp. workaround for iOS where buffersizes <512 lead to not sample data being delivered?
    m_deviceManager.setAudioDeviceSetup(currentSetup, false);
#endif
    m_deviceManager.addAudioCallback(&m_processor);

    m_body->setProcessor(&m_processor);

	setSize(500, 220);
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
	auto panelDefaultSize = 40.0f;
    auto isPortrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();
    if(m_body)
        m_body->setPortrait(isPortrait);
    
    auto safety = getDeviceSafetyMargins();
	
	FlexBox fb;
	if(isPortrait)
	{
        m_header->setNoGoArea(safety._top, safety._bottom, safety._left, safety._right);
        m_footer->setNoGoArea(safety._top, safety._bottom, safety._left, safety._right);
        
		fb.flexDirection = FlexBox::Direction::column;
		fb.items.addArray({
            FlexItem(*m_header.get()).withFlex(1).withMaxHeight(panelDefaultSize + safety._top),
            FlexItem(*m_body.get()).withFlex(4),
            FlexItem(*m_footer.get()).withFlex(1).withMaxHeight(panelDefaultSize + safety._bottom) });
	}
	else
	{
        m_header->setNoGoArea(safety._top, safety._bottom, safety._left, safety._right);
        m_footer->setNoGoArea(safety._top, safety._bottom, safety._left, safety._right);
        
		fb.flexDirection = FlexBox::Direction::row;
		fb.items.addArray({
            FlexItem(*m_header.get()).withFlex(1).withMaxWidth(panelDefaultSize + safety._left),
            FlexItem(*m_body.get()).withFlex(4),
            FlexItem(*m_footer.get()).withFlex(1).withMaxWidth(panelDefaultSize + safety._right) });
	}

    fb.performLayout(getLocalBounds().toFloat());

}

void MainComponent::onPauseProcessing(bool pause)
{
	m_processor.setPauseProcessing(pause);
}

AudioSelectComponent* MainComponent::onOpenAudioConfig()
{
	if (!m_audioConfig)
	{
		int  				minAudioInputChannels = 1;
		int  				maxAudioInputChannels = INT_MAX;
		int  				minAudioOutputChannels = 0;
		int  				maxAudioOutputChannels = 0;
		bool  				showMidiInputOptions = false;
		bool  				showMidiOutputSelector = false;
		bool  				showChannelsAsStereoPairs = false;
		bool  				hideAdvancedOptionsWithButton = false;

		m_audioConfig = std::make_unique<AudioSelectComponent>(m_deviceManager,
			minAudioInputChannels,
			maxAudioInputChannels,
			minAudioOutputChannels,
			maxAudioOutputChannels,
			showMidiInputOptions,
			showMidiOutputSelector,
			showChannelsAsStereoPairs,
			hideAdvancedOptionsWithButton);

		addAndMakeVisible(m_audioConfig.get());

		return m_audioConfig.get();
	}
    else
    {
        removeChildComponent(m_audioConfig.get());
        m_audioConfig = nullptr;

        return nullptr;
    }
}

VisuSelectComponent* MainComponent::onOpenVisuConfig()
{
	if (!m_visuConfig)
	{
		m_visuConfig = std::make_unique<VisuSelectComponent>();
        m_visuConfig->addListener(m_body.get());

		addAndMakeVisible(m_visuConfig.get());

		return m_visuConfig.get();
	}
    else
    {
        removeChildComponent(m_visuConfig.get());
        m_visuConfig = nullptr;

        return nullptr;
    }
}
