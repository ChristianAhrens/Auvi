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


//==============================================================================
/*
*/
class VisuSelectComponent : public Component,
                            public TextButton::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        virtual void onUpdateVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes) = 0;
    };
    
public:
    VisuSelectComponent();
    ~VisuSelectComponent();
    
    void addListener(Listener *l);

    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    std::map<AbstractAudioVisualizer::VisuType, std::unique_ptr<TextButton>> m_visuSelectButtons;
    Listener *m_listener;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisuSelectComponent)
};
