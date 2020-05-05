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

namespace Auvi
{

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
        virtual VisuSelectComponent* onOpenVisuConfigSelect() = 0;
        virtual VisuSelectComponent* getVisuConfigSelect() = 0;
    };

    static constexpr const char* VISU_CONFIG_OPEN_ID = "VISU_CONFIG_OPEN_ID";
    
public:
    Footer(int noGoAreaTop = 0, int noGoAreaBottom = 0, int noGoAreaLeft = 0, int noGoAreaRight = 0);
    ~Footer();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
	void buttonClicked(Button* button) override;

    //==============================================================================
    void setNoGoArea(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight);

private:
    std::unique_ptr<TextButton> m_visuConfigOpen;
    int                         m_noGoAreaTop;
    int                         m_noGoAreaBottom;
    int                         m_noGoAreaLeft;
    int                         m_noGoAreaRight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Footer)
};

}