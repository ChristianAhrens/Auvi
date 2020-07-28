/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "Processor/Processor.h"

#include "../submodules/JUCE-AppBasics/Source/iOS_utils.hpp"

#include "AppConfiguration.h"
#include "utils.hpp"

namespace Auvi
{

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

	m_processor			= std::make_unique<Processor>();
	m_deviceManager		= std::make_unique<AudioDeviceManager>();

	m_deviceManager->addAudioCallback(m_processor.get());
	m_body->setProcessor(m_processor.get());

	m_config			= std::make_unique<AppConfiguration>(JUCEAppBasics::AppConfigurationBase::getDefaultConfigFilePath());
	m_config->addListener(this);
	if(!m_config->isValid())
	{
		// Hacky bit of device manager initialization:
		// We first intialize it to be able to get a valid device setup,
		// then initialize with a dummy xml config to trigger the internal xml structure being reset
		// and finally apply the original initialized device setup again to have the audio running correctly.
		// If we did not do so, either the internal xml would not be present as long as the first configuration change was made
		// and therefor no valid config file could be written by Auvi or the audio would not be running
		// on first start and manual config would be required.
		m_deviceManager->initialiseWithDefaultDevices(Auvi::utils::max_input_channels, 0);
		auto audioDeviceSetup = m_deviceManager->getAudioDeviceSetup();
        m_deviceManager->initialise(Auvi::utils::max_input_channels, 0, std::make_unique<XmlElement>(AppConfiguration::getTagName(AppConfiguration::TagID::DEVCFG)).get(), false, {}, nullptr);
#if JUCE_IOS
		if(audioDeviceSetup.bufferSize < 512)
			audioDeviceSetup.bufferSize = 512; // temp. workaround for iOS where buffersizes <512 lead to no sample data being delivered?
#endif
		m_deviceManager->setAudioDeviceSetup(audioDeviceSetup, true);

		// Default visualizer shall be our scope
		m_body->onUpdateVisuTypes(std::set<AbstractAudioVisualizer::VisuType>{ AbstractAudioVisualizer::VisuType::Scope });

		triggerConfigurationUpdate();
	}
	else
	{
		auto devMgrConfigState = m_config->getConfigState(AppConfiguration::getTagName(AppConfiguration::TagID::DEVCFG));
		auto visuConfigState = m_config->getConfigState(AppConfiguration::getTagName(AppConfiguration::TagID::GUI));

		if (devMgrConfigState)
			m_deviceManager->initialise(2, 0, devMgrConfigState.get(), true);

		if (visuConfigState)
			setStateXml(visuConfigState.get());
	}


	addMouseListener(this, true);

	setSize(500, 220);
}

MainComponent::~MainComponent()
{
	m_deviceManager->removeAudioCallback(m_processor.get());
}

std::unique_ptr<XmlElement> MainComponent::createStateXml()
{
	auto guiXmlElement = std::make_unique<XmlElement>(AppConfiguration::getTagName(AppConfiguration::TagID::GUI));
	if (m_body)
		guiXmlElement->addChildElement(m_body->createStateXml().release());

	return std::make_unique<XmlElement>(*guiXmlElement);
}

bool MainComponent::setStateXml(XmlElement *stateXml)
{
	if (m_body)
	{
		auto visuXmlElement = stateXml->getChildByName(AppConfiguration::getTagName(AppConfiguration::TagID::VISU));
		if (visuXmlElement)
			return m_body->setStateXml(visuXmlElement);
		else
			return false;
	}
	else
		return false;
}

void MainComponent::performConfigurationDump()
{
	auto configuration = AppConfiguration::getInstance();
	if (configuration != nullptr)
	{
		configuration->setConfigState(m_deviceManager->createStateXml());
		configuration->setConfigState(createStateXml());
	}
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
    
    auto safety = JUCEAppBasics::iOS_utils::getDeviceSafetyMargins();
	
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

void MainComponent::mouseDown(const MouseEvent& event)
{
	// if audio config currently is opened and a click is performed outside of it, close it
	if (m_audioConfig && (event.eventComponent->getComponentID() != String(Header::AUDIO_CONFIG_OPEN_ID)) && !m_audioConfig->getBounds().contains(event.getEventRelativeTo(this).getMouseDownPosition()))
		onToggleAudioConfigSelect();
	// if visu config currently is opened and a click is performed outside of it, close it
	if (m_visuConfig && (event.eventComponent->getComponentID() != String(Footer::VISU_CONFIG_OPEN_ID)) && !m_visuConfig->getBounds().contains(event.getEventRelativeTo(this).getMouseDownPosition()))
		onToggleVisuConfigSelect();

	Component::mouseDown(event);
}

void MainComponent::onPauseProcessing(bool pause)
{
	m_processor->setPauseProcessing(pause);
}

AudioSelectComponent* MainComponent::onToggleAudioConfigSelect()
{
	if (!m_audioConfig)
	{
		int  				minAudioInputChannels = 1;
		int  				maxAudioInputChannels = Auvi::utils::max_input_channels;
		int  				minAudioOutputChannels = 0;
		int  				maxAudioOutputChannels = 0;
		bool  				showMidiInputOptions = false;
		bool  				showMidiOutputSelector = false;
		bool  				showChannelsAsStereoPairs = false;
		bool  				hideAdvancedOptionsWithButton = true;

		m_audioConfig = std::make_unique<AudioSelectComponent>(m_deviceManager.get(),
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
		triggerConfigurationUpdate();

        removeChildComponent(m_audioConfig.get());
        m_audioConfig = nullptr;

        return nullptr;
    }
}

AudioSelectComponent* MainComponent::getAudioConfigSelect()
{
	if (m_audioConfig)
		return m_audioConfig.get();
	else
		return nullptr;
}

VisuSelectComponent* MainComponent::onToggleVisuConfigSelect()
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

		triggerConfigurationUpdate();

        return nullptr;
    }
}

VisuSelectComponent* MainComponent::getVisuConfigSelect()
{
	if (m_visuConfig)
		return m_visuConfig.get();
	else
		return nullptr;
}

}
