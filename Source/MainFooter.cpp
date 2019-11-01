/*
  ==============================================================================

    MainFooter.cpp
    Created: 1 Nov 2019 11:35:51am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainFooter.h"

//==============================================================================
MainFooter::MainFooter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	
}

MainFooter::~MainFooter()
{
}

void MainFooter::paint (Graphics& g)
{
	//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).overlaidWith(
	//	getLookAndFeel().findColour(ResizableWindow::backgroundColourId)));
	g.fillAll(Colours::green);
}

void MainFooter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
