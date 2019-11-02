/*
  ==============================================================================

    MainHeader.cpp
    Created: 1 Nov 2019 11:34:46am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHeader.h"

//==============================================================================
MainHeader::MainHeader()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MainHeader::~MainHeader()
{
}

void MainHeader::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void MainHeader::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
