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
    AbstractAudioVisualizer::paint(g);

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
    AbstractAudioVisualizer::resized();
}

AbstractAudioVisualizer::VisuType WaterfallAudioVisualizer::getType()
{
    return AbstractAudioVisualizer::VisuType::Waterfall;
}

void WaterfallAudioVisualizer::processingDataChanged(AbstractProcessorData *data)
{
    ignoreUnused(data);
}
