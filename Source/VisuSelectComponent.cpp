/*
  ==============================================================================

    VisuSelectComponent.cpp
    Created: 19 Nov 2019 7:33:56pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "VisuSelectComponent.h"

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
VisuSelectComponent::VisuSelectComponent()
	: Component()
{
    m_listener = 0;
    
    for(int i=AbstractAudioVisualizer::VisuType::InvalidFirst+1; i<AbstractAudioVisualizer::VisuType::InvalidLast; ++i)
    {
        m_visuSelectButtons[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<TextButton>();
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->setButtonText(String(i));
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->setClickingTogglesState(true);
        addAndMakeVisible(m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i).get());
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->addListener(this);
    }
}

VisuSelectComponent::~VisuSelectComponent()
{
    //Listener* acListener = dynamic_cast<Listener*>(getParentComponent());
    //if (acListener)
    if(m_listener)
    {
        std::set<AbstractAudioVisualizer::VisuType> activeVisuTypes;
        for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<TextButton>> &p : m_visuSelectButtons)
        {
            if(p.second->getToggleState())
                activeVisuTypes.insert(p.first);
        }
        
        //acListener->onUpdateVisuTypes(activeVisuTypes);
        m_listener->onUpdateVisuTypes(activeVisuTypes);
    }
}

void VisuSelectComponent::addListener(Listener *l)
{
    m_listener = l;
}

void VisuSelectComponent::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	Component::paint(g);
}

void VisuSelectComponent::resized()
{
    Grid grid;
    using Track = Grid::TrackInfo;
    grid.templateRows    = { Track (1_fr), Track (1_fr) };
    grid.templateColumns = { Track (1_fr), Track (1_fr), Track (1_fr) };
    for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<TextButton>> &p : m_visuSelectButtons)
    {
        grid.items.add(GridItem(p.second.get()));
    }
    grid.performLayout (getLocalBounds());
}

void VisuSelectComponent::buttonClicked(Button* button)
{
    if (/*m_audioConfigOpen && m_audioConfigOpen.get() ==*/ button)
    {
        //Listener* acListener = dynamic_cast<Listener*>(getParentComponent());
        //if (acListener)
        if(m_listener)
        {
        }
    }
}
