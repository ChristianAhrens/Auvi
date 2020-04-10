/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Header.h"
#include "Footer.h"
#include "Body.h"

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
                        public Footer::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;
    
    //==============================================================================
    VisuSelectComponent * onOpenVisuConfigSelect() override;
    VisuSelectComponent* getVisuConfigSelect() override;

	//==============================================================================
	AudioSelectComponent * onOpenAudioConfigSelect() override;
    AudioSelectComponent* getAudioConfigSelect() override;
    void onPauseProcessing(bool pause) override;

private:
    //==============================================================================
    std::unique_ptr<Header>						m_header;
    std::unique_ptr<Body>                       m_body;
    std::unique_ptr<Footer>						m_footer;

	std::unique_ptr<AudioSelectComponent>		m_audioConfig;
	std::unique_ptr<VisuSelectComponent>        m_visuConfig;

	Processor									m_processor;
	AudioDeviceManager							m_deviceManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
