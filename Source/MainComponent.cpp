/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

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
    
    m_deviceManager.initialiseWithDefaultDevices(1, 0);
    m_deviceManager.addAudioCallback(&m_processor);

    m_body->setProcessor(&m_processor);

	setSize(500, 580);
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
    if(m_body)
        m_body->setPortrait(isPortrait);
	
	FlexBox fb;
	if(isPortrait)
	{
		fb.flexDirection = FlexBox::Direction::column;
		fb.items.addArray({ FlexItem(*m_header.get()).withFlex(1).withMaxHeight(panelMaxSize),
						    FlexItem(*m_body.get()).withFlex(4),
							FlexItem(*m_footer.get()).withFlex(1).withMaxHeight(panelMaxSize) });
	}
	else
	{
		fb.flexDirection = FlexBox::Direction::row;
		fb.items.addArray({ FlexItem(*m_header.get()).withFlex(1).withMaxWidth(panelMaxSize),
					        FlexItem(*m_body.get()).withFlex(4),
							FlexItem(*m_footer.get()).withFlex(1).withMaxWidth(panelMaxSize) });
	}
	fb.performLayout(getLocalBounds().toFloat());
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
