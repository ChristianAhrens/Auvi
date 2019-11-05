/*
  ==============================================================================

    Header.cpp
    Created: 1 Nov 2019 11:34:46am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Header.h"

//==============================================================================
Header::Header()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Header::~Header()
{
}

void Header::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void Header::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
