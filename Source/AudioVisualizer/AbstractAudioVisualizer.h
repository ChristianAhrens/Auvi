/*
  ==============================================================================

    AbstractAudioVisualizer.h
    Created: 1 Nov 2019 11:39:06am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../Processor/Processor.h"

//==============================================================================
/*
*/
class AudioVisualizerConfigBase : public Component, public DrawableButton::Listener
{
public:
    AudioVisualizerConfigBase();
    ~AudioVisualizerConfigBase();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    void buttonClicked(Button* button) override;

private:


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioVisualizerConfigBase)
};

//==============================================================================
/*
*/
class AbstractAudioVisualizer : public Component, public Processor::Listener, public DrawableButton::Listener
{
public:
    enum VisuType
    {
        InvalidFirst,
        MultiMeter,
        Scope,
        TwoDField,
        Rta,
        Waterfall,
        Waveform,
        InvalidLast
    };

    static constexpr const char* VISUALIZER_CONFIG_OPEN_ID = "VISUALIZER_CONFIG_OPEN_ID";

public:
    AbstractAudioVisualizer();
    ~AbstractAudioVisualizer();

    void showConfigButton(bool enable);
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;

    //==============================================================================
    void buttonClicked(Button* button) override;
    
    //==============================================================================
    virtual VisuType getType() = 0;
    virtual std::unique_ptr<AudioVisualizerConfigBase> openAudioVisualizerConfig();
    
    //==============================================================================
    static std::string VisuTypeToString(VisuType type);

private:
    void onOpenConfigClicked();

    std::unique_ptr<DrawableButton>	m_openConfig;
    std::unique_ptr<AudioVisualizerConfigBase> m_visualizerConfig;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AbstractAudioVisualizer)
};
