/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "Header.h"
#include "Footer.h"
#include "Processor/Processor.h"
#include "AudioVisualizer/TwoDFieldAudioVisualizer.h"
#include "AudioVisualizer/MultiMeterAudioVisualizer.h"
#include "AudioVisualizer/ScopeAudioVisualizer.h"
#include "AudioVisualizer/RtaAudioVisualizer.h"
#include "AudioVisualizer/WaterfallAudioVisualizer.h"
#include "AudioVisualizer/WaveformAudioVisualizer.h"

//==============================================================================
MainComponent::MainComponent()
{
	m_deviceManager.initialiseWithDefaultDevices(1, 0);
	m_deviceManager.addAudioCallback(&m_Processor);

	m_audioConfig = nullptr;

    m_Header            = std::make_unique<Header>();
	addAndMakeVisible(m_Header.get());
    m_Footer            = std::make_unique<Footer>();
	addAndMakeVisible(m_Footer.get());
    
    onUpdateVisuType(AbstractAudioVisualizer::VisuType::Scope);

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

void MainComponent::onUpdateVisuType(AbstractAudioVisualizer::VisuType type)
{
    m_Processor.RemoveListener(m_AudioVisualizer.get());
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
    m_Processor.AddListener(m_AudioVisualizer.get());
    
    resized();
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
