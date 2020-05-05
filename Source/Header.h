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

namespace Auvi
{

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
        virtual AudioSelectComponent* onOpenAudioConfigSelect() = 0;
        virtual AudioSelectComponent* getAudioConfigSelect() = 0;
        virtual void onPauseProcessing(bool pause) = 0;
    };

    static constexpr const char* AUDIO_CONFIG_OPEN_ID = "AUDIO_CONFIG_OPEN_ID";
    static constexpr const char* STOP_PROCESSING_ID = "STOP_PROCESSING_ID";
    
public:
    Header(int noGoAreaTop = 0, int noGoAreaBottom = 0, int noGoAreaLeft = 0, int noGoAreaRight = 0);
    ~Header();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
	void buttonClicked(Button* button) override;

    //==============================================================================
    void setNoGoArea(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight);

private:
	std::unique_ptr<TextButton>	m_audioConfigOpen;
    std::unique_ptr<DrawableButton>	m_stopProcessing;
    int                         m_noGoAreaTop;
    int                         m_noGoAreaBottom;
    int                         m_noGoAreaLeft;
    int                         m_noGoAreaRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};

}