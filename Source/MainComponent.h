/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Header.h"
#include "Footer.h"

//==============================================================================
/* Fwd declarations */
class AbstractAudioVisualizer;
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
    
    //==============================================================================
    void onUpdateVisuType(AbstractAudioVisualizer::VisuType type) override;

private:
    //==============================================================================
    std::unique_ptr<Header>                     m_Header;
    std::unique_ptr<AbstractAudioVisualizer>    m_AudioVisualizer;
    std::unique_ptr<Footer>                     m_Footer;
    
    std::unique_ptr<Processor>                  m_Processor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
