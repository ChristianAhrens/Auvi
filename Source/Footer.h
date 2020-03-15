/*
  ==============================================================================

    MainFooter.h
    Created: 1 Nov 2019 11:35:51am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "VisuSelectComponent.h"

#include "AudioVisualizer/AbstractAudioVisualizer.h"

//==============================================================================
/*
*/
class Footer : public Component, public TextButton::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        virtual VisuSelectComponent * onOpenVisuConfig() = 0;
    };
    
public:
    Footer(int noGoAreaBottom = 0, int noGoAreaLeft = 0);
    ~Footer();

    void paint (Graphics&) override;
    void resized() override;
    
	void buttonClicked(Button* button) override;
    void setNoGoArea(int noGoAreaBottom, int noGoAreaLeft);

private:
    std::unique_ptr<TextButton> m_visuConfigOpen;
	VisuSelectComponent*        m_visuConfigSelect;
    int                         m_noGoAreaBottom;
    int                         m_noGoAreaLeft;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Footer)
};
