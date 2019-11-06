/*
  ==============================================================================

    WaveformAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "WaveformAudioVisualizer.h"

//==============================================================================
WaveformAudioVisualizer::WaveformAudioVisualizer()
    : AbstractAudioVisualizer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveformAudioVisualizer::~WaveformAudioVisualizer()
{
}

void WaveformAudioVisualizer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("WaveformAudioVisualizer", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void WaveformAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType WaveformAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Waveform;
}

void WaveformAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    ignoreUnused(data);
}