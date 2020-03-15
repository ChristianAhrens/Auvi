/*
  ==============================================================================

    MainHeader.h
    Created: 1 Nov 2019 11:34:46am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioSelectComponent.h"

//==============================================================================
/*
*/
class Header    : public Component, public TextButton::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        virtual AudioSelectComponent * onOpenAudioConfig() = 0;
    };
    
public:
    Header(int noGoAreaTop = 0, int noGoAreaLeft = 0);
    ~Header();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* button) override;
    void setNoGoArea(int noGoAreaTop, int noGoAreaLeft);

private:
	std::unique_ptr<TextButton>	m_audioConfigOpen;
	AudioSelectComponent*		m_audioConfigSelect;
    int                         m_noGoAreaTop;
    int                         m_noGoAreaLeft;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};
