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
    
    m_deviceManager.initialiseWithDefaultDevices(2, 0);
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
    auto orientation = Desktop::getInstance().getCurrentOrientation();
    auto isPortrait = (orientation == Desktop::upright) || (orientation == Desktop::upsideDown);
    if(m_body)
        m_body->setPortrait(isPortrait);
    
    auto topSafety = 0.0f;
    auto bottomSafety = 0.0f;
    auto leftSafety = 0.0f;
    auto rightSafety = 0.0f;
    switch(SystemStats::getOperatingSystemType())
    {
        case SystemStats::OperatingSystemType::Android:
        case SystemStats::OperatingSystemType::iOS:
            {
                auto displayNotch = 35.0f;
                auto displaySlideBar = 20.0f;
                switch(orientation)
                {
                    case Desktop::upright:
                        topSafety = displayNotch;
                        bottomSafety = displaySlideBar;
                        break;
                    case Desktop::upsideDown:
                        topSafety = displaySlideBar;
                        bottomSafety = displayNotch;
                        break;
                    case Desktop::rotatedAntiClockwise:
                        leftSafety = displayNotch;
                        rightSafety = displaySlideBar;
                        break;
                    case Desktop::rotatedClockwise:
                        leftSafety = displaySlideBar;
                        rightSafety = displayNotch;
                        break;
                    default:
                        break;
                }
            }
            break;
        case SystemStats::OperatingSystemType::MacOSX:
        case SystemStats::OperatingSystemType::Windows:
        case SystemStats::OperatingSystemType::Linux:
        default:
            break;
    }
	
	FlexBox fb;
	if(isPortrait)
	{
        m_header->setNoGoArea(topSafety, bottomSafety, leftSafety, rightSafety);
        m_footer->setNoGoArea(topSafety, bottomSafety, leftSafety, rightSafety);
        
		fb.flexDirection = FlexBox::Direction::column;
		fb.items.addArray({
            FlexItem(*m_header.get()).withFlex(1).withMaxHeight(panelDefaultSize + topSafety),
            FlexItem(*m_body.get()).withFlex(4),
            FlexItem(*m_footer.get()).withFlex(1).withMaxHeight(panelDefaultSize + bottomSafety) });
	}
	else
	{
        m_header->setNoGoArea(topSafety, bottomSafety, leftSafety, rightSafety);
        m_footer->setNoGoArea(topSafety, bottomSafety, leftSafety, rightSafety);
        
		fb.flexDirection = FlexBox::Direction::row;
		fb.items.addArray({
            FlexItem(*m_header.get()).withFlex(1).withMaxWidth(panelDefaultSize + leftSafety),
            FlexItem(*m_body.get()).withFlex(4),
            FlexItem(*m_footer.get()).withFlex(1).withMaxWidth(panelDefaultSize + rightSafety) });
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
