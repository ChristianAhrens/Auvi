/*
  ==============================================================================

    VisuSelectComponent.h
    Created: 19 Nov 2019 7:33:56pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <set>

#include "AudioVisualizer/AbstractAudioVisualizer.h"

namespace Auvi
{

//==============================================================================
/*
*/
class VisuSelectComponent : public Component,
                            public DrawableButton::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        virtual void onUpdateVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes) = 0;
		virtual const std::set<AbstractAudioVisualizer::VisuType> getActiveVisuTypes() = 0;
    };
    
public:
    VisuSelectComponent();
	VisuSelectComponent(std::set<AbstractAudioVisualizer::VisuType> visuTypes);
    ~VisuSelectComponent();

	void setActiveVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes);
    
    void addListener(Listener *l);

    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    std::unique_ptr<Drawable> getVisuTypeDrawable(AbstractAudioVisualizer::VisuType type);
    
    std::map<AbstractAudioVisualizer::VisuType, std::unique_ptr<DrawableButton>> m_visuSelectButtons;
    Listener *m_listener;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisuSelectComponent)
};

}