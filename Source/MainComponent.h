/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "MainHeader.h"
#include "MainFooter.h"

//==============================================================================
/* Fwd declarations */
class MainHeader;
class AbstractAudioVisualizer;
class MainFooter;
class MainProcessor;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
                        public MainHeader::Listener,
                        public MainFooter::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void updateVisuType(AbstractAudioVisualizer::VisuType type);

private:
    //==============================================================================
    std::unique_ptr<MainHeader>                 m_Header;
    std::unique_ptr<AbstractAudioVisualizer>    m_AudioVisualizer;
    std::unique_ptr<MainFooter>                 m_Footer;
    
    std::unique_ptr<MainProcessor>              m_Processor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
