/*
  ==============================================================================

    AbstractAudioVisualizer.cpp
    Created: 1 Nov 2019 11:39:06am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AbstractAudioVisualizer.h"

//==============================================================================
AbstractAudioVisualizer::AbstractAudioVisualizer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

AbstractAudioVisualizer::~AbstractAudioVisualizer()
{
}

void AbstractAudioVisualizer::paint (Graphics& g)
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
    g.drawText ("AbstractAudioVisualizer", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void AbstractAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

std::string AbstractAudioVisualizer::VisuTypeToString(VisuType type)
{
    switch(type)
    {
        case MultiMeter:
            return "MultiMeter";
        case Scope:
            return "Scope";
        case TwoDField:
            return "TwoDField";
        case Rta:
            return "RTA";
        case Waterfall:
            return "Waterfall";
        case Waveform:
            return "Waveform";
        case InvalidFirst:
        case InvalidLast:
        default:
            return "Invalid";
    }
}
