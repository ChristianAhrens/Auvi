/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Header.h"
#include "Footer.h"
#include "Body.h"

namespace Auvi
{

//==============================================================================
/* Fwd declarations */
class Processor;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
                        public Header::Listener,
                        public Footer::Listener,
                        public AppConfiguration::XmlConfigurableElement,
                        public AppConfiguration::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    std::unique_ptr<XmlElement> createStateXml() override;
    bool setStateXml(XmlElement* stateXml) override;

    //==============================================================================
    void performConfigurationDump() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;
    
    //==============================================================================
    VisuSelectComponent * onToggleVisuConfigSelect() override;
    VisuSelectComponent* getVisuConfigSelect() override;

	//==============================================================================
	AudioSelectComponent * onToggleAudioConfigSelect() override;
    AudioSelectComponent* getAudioConfigSelect() override;
    void onPauseProcessing(bool pause) override;

private:
    //==============================================================================
    std::unique_ptr<Header>						m_header;
    std::unique_ptr<Body>                       m_body;
    std::unique_ptr<Footer>						m_footer;

	std::unique_ptr<AudioSelectComponent>		m_audioConfig;
	std::unique_ptr<VisuSelectComponent>        m_visuConfig;

    std::unique_ptr<Processor>					m_processor;
	std::unique_ptr<AudioDeviceManager>			m_deviceManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

}