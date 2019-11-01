/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "MainHeader.h"
#include "AudioVisualizer/TwoDFieldAudioVisualizer.h"
#include "AudioVisualizer/MultiMeterAudioVisualizer.h"
#include "AudioVisualizer/ScopeAudioVisualizer.h"
#include "MainFooter.h"
#include "MainProcessor.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    
    m_Header            = std::make_unique<MainHeader>();
    m_AudioVisualizer   = std::make_unique<TwoDFieldAudioVisualizer>();
    m_Footer            = std::make_unique<MainFooter>();
    
    m_Processor         = std::make_unique<MainProcessor>();
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
