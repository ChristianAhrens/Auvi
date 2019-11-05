/*
  ==============================================================================

    MainFooter.h
    Created: 1 Nov 2019 11:35:51am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioVisualizer/AbstractAudioVisualizer.h"

//==============================================================================
/*
*/
class Footer : public Component, public ComboBox::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        virtual void onUpdateVisuType(AbstractAudioVisualizer::VisuType type) = 0;
    };
    
public:
    Footer();
    ~Footer();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;

private:
    std::unique_ptr<ComboBox>   m_visuTypeSelect;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Footer)
};
