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
#include "../AppConfiguration.h"
#include "../SplitButtonComponent.h"

namespace Auvi
{

//==============================================================================
/*
*/
class AudioVisualizerConfigBase : public Component, public SplitButtonComponent::Listener
{
public:
    enum MappingKey
    {
        invalid,
        L,
        R,
        C,
        LS,
        RS,
        LFE,
        X,
        Y,
        RTA
    };
    static std::string getMappingString(MappingKey key)
    {
        switch (key)
        {
            case L:
                return "Left";
            case R:
                return "Right";
            case C:
                return "Center";
            case LS:
                return "Left Surround";
            case RS:
                return "Right Surround";
            case LFE:
                return "LFE";
            case X:
                return "X";
            case Y:
                return "Y";
            case RTA:
                return "Analyzer channel";
            default:
                return std::string{};
        }
    }

public:
    AudioVisualizerConfigBase(std::map<AudioVisualizerConfigBase::MappingKey, int> mapping = std::map<AudioVisualizerConfigBase::MappingKey, int>{}, bool usesValuesInDB = false);
    ~AudioVisualizerConfigBase();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    //==============================================================================
    void buttonClicked(uint64 buttonId) override;

    //==============================================================================
    void setChannelMapping(std::map<AudioVisualizerConfigBase::MappingKey, int> mapping);
    std::map<AudioVisualizerConfigBase::MappingKey, int> const getChannelMapping();

    //==============================================================================
    void setUsesValuesInDB(bool useValuesInDB);
    bool getUsesValuesInDB();

private:
    std::map<AudioVisualizerConfigBase::MappingKey, std::unique_ptr<Label>>     m_visualizerMappingLabels;
    std::map<AudioVisualizerConfigBase::MappingKey, std::unique_ptr<ComboBox>>  m_visualizerMappingSelects;

    std::unique_ptr<SplitButtonComponent>     m_usesValuesInDBSplitButton;
    bool m_usesValuesInDB{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioVisualizerConfigBase)
};

//==============================================================================
/*
*/
class AbstractAudioVisualizer : public Component, 
                                public Processor::Listener, 
                                public DrawableButton::Listener, 
                                public AppConfiguration::XmlConfigurableElement
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
    virtual ~AbstractAudioVisualizer();

    void showConfigButton(bool enable);
    void notifyChanges();
    void processChanges();

    std::unique_ptr<XmlElement> createStateXml() override;
    bool setStateXml(XmlElement* stateXml) override;
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;

    //==============================================================================
    void buttonClicked(Button* button) override;
    
    //==============================================================================
    virtual VisuType getType() = 0;
    virtual void processChangedChannelMapping() = 0;
    virtual std::unique_ptr<AudioVisualizerConfigBase> openAudioVisualizerConfig();
    virtual void closeAudioVisualizerConfig();
    
    //==============================================================================
    static std::string VisuTypeToString(VisuType type);
    static VisuType StringToVisuType(std::string typeName);

protected:
    std::map<AudioVisualizerConfigBase::MappingKey, int> m_channelMapping;
    bool m_usesValuesInDB{ 0 };

private:
    void onOpenConfigClicked();

    std::unique_ptr<DrawableButton>	m_openConfig;
    std::unique_ptr<AudioVisualizerConfigBase> m_visualizerConfig;
    bool m_changesPending;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AbstractAudioVisualizer)
};

}
