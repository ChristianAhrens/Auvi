/*
  ==============================================================================

    WaterfallAudioVisualizer.cpp
    Created: 1 Nov 2019 11:43:01am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "WaterfallAudioVisualizer.h"

//==============================================================================
WaterfallAudioVisualizer::WaterfallAudioVisualizer()
    : AbstractAudioVisualizer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaterfallAudioVisualizer::~WaterfallAudioVisualizer()
{
}

void WaterfallAudioVisualizer::paint (Graphics& g)
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
    g.drawText ("WaterfallAudioVisualizer", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void WaterfallAudioVisualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

AbstractAudioVisualizer::VisuType WaterfallAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Waterfall;
}

void WaterfallAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    ignoreUnused(data);
}
