/*
  ==============================================================================

    Footer.cpp
    Created: 1 Nov 2019 11:35:51am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Footer.h"
#include "AudioVisualizer/AbstractAudioVisualizer.h"

//==============================================================================
Footer::Footer()
{
    m_visuTypeSelect = std::make_unique<ComboBox>();
    addAndMakeVisible(m_visuTypeSelect.get());
    m_visuTypeSelect->addListener(this);
    for(int i = AbstractAudioVisualizer::VisuType::InvalidFirst + 1; i < AbstractAudioVisualizer::VisuType::InvalidLast; ++i)
    {
        m_visuTypeSelect->addItem(AbstractAudioVisualizer::VisuTypeToString(AbstractAudioVisualizer::VisuType(i)), i);
    }
    m_visuTypeSelect->setSelectedId(AbstractAudioVisualizer::VisuType::MultiMeter);
}

Footer::~Footer()
{
}

void Footer::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void Footer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    m_visuTypeSelect->setBounds(10,10,150,20);
}

void Footer::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if(m_visuTypeSelect && m_visuTypeSelect.get()==comboBoxThatHasChanged)
    {
        Listener* mcListener = dynamic_cast<Listener*>(getParentComponent());
        if(mcListener)
            mcListener->onUpdateVisuType(AbstractAudioVisualizer::VisuType(m_visuTypeSelect->getSelectedId()));
    }
}
