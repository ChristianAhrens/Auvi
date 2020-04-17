/*
  ==============================================================================

    AbstractAudioVisualizer.cpp
    Created: 1 Nov 2019 11:39:06am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "AbstractAudioVisualizer.h"

//==============================================================================
AudioVisualizerConfigBase::AudioVisualizerConfigBase(std::map<std::string, int> mapping)
{
    m_visualizerMappingSelects = std::map<std::string, std::unique_ptr<ComboBox>>{};
    m_visualizerMappingLabels = std::map<std::string, std::unique_ptr<Label>>{};

    setChannelMapping(mapping);
}

AudioVisualizerConfigBase::~AudioVisualizerConfigBase()
{
}

void AudioVisualizerConfigBase::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    if(m_visualizerMappingSelects.empty())
    {
        g.setColour(Colours::white);
        g.setFont(14.0f);
        g.drawText("No configuration implemtented for this AudioVisualizer", getLocalBounds(),
            Justification::centred, true);   // draw some placeholder text
    }
    else
    {

    }
}

void AudioVisualizerConfigBase::resized()
{

    auto itemHeight = 19;
    auto space = itemHeight / 4;
    Rectangle<int> r(proportionOfWidth(0.35f), space, proportionOfWidth(0.6f), getHeight());
    for(auto iter = m_visualizerMappingSelects.cbegin(); iter != m_visualizerMappingSelects.cend(); iter++)
    {
        auto rect = r.removeFromTop(itemHeight);
        if(m_visualizerMappingLabels.count(iter->first)!=0)
            m_visualizerMappingLabels.at(iter->first)->setBounds(rect.withX(0).withWidth(proportionOfWidth(0.6f)));
        if (m_visualizerMappingSelects.count(iter->first) != 0)
            m_visualizerMappingSelects.at(iter->first)->setBounds(rect.withX(proportionOfWidth(0.6f)).withWidth(proportionOfWidth(0.4f) - space));
        r.removeFromTop(space);
    }

    setSize(getWidth(), r.getY());
}

void AudioVisualizerConfigBase::setChannelMapping(std::map<std::string, int> mapping)
{
    m_visualizerMappingLabels.clear();
    m_visualizerMappingSelects.clear();
    for (auto NameChannelKV : mapping)
    {
        m_visualizerMappingLabels.insert(std::make_pair(NameChannelKV.first, std::make_unique<Label>()));
        m_visualizerMappingLabels.at(NameChannelKV.first)->setText(NameChannelKV.first, dontSendNotification);
        addAndMakeVisible(m_visualizerMappingLabels.at(NameChannelKV.first).get());
        m_visualizerMappingSelects.insert(std::make_pair(NameChannelKV.first, std::make_unique<ComboBox>()));
        addAndMakeVisible(m_visualizerMappingSelects.at(NameChannelKV.first).get());
        m_visualizerMappingSelects.at(NameChannelKV.first)->addItemList({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" }, 1);
        m_visualizerMappingSelects.at(NameChannelKV.first)->setSelectedId(NameChannelKV.second, dontSendNotification);
    }
}

std::map<std::string, int> const AudioVisualizerConfigBase::getChannelMapping()
{
    std::map<std::string, int> visualizerChannelMapping;
    for (auto iter = m_visualizerMappingSelects.cbegin(); iter != m_visualizerMappingSelects.cend(); iter++)
    {
        visualizerChannelMapping.insert(std::make_pair(iter->first, iter->second->getSelectedId()));
    }

    return visualizerChannelMapping;
}

//==============================================================================
AbstractAudioVisualizer::AbstractAudioVisualizer()
{
    m_channelMapping = std::map<std::string, int>{};

    m_openConfig = std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageStretched);
    m_openConfig->setComponentID(VISUALIZER_CONFIG_OPEN_ID);
    m_openConfig->setClickingTogglesState(true);
    std::unique_ptr<XmlElement> Settings_svg_xml = XmlDocument::parse(BinaryData::settings24px_svg);

    std::unique_ptr<juce::Drawable> drawableSettingsNormalImage = Drawable::createFromSVG(*(Settings_svg_xml.get()));
    drawableSettingsNormalImage->replaceColour(Colours::black, Colours::white);
    std::unique_ptr<juce::Drawable> drawableSettingsOverImage = Drawable::createFromSVG(*(Settings_svg_xml.get()));
    drawableSettingsOverImage->replaceColour(Colours::black, Colours::lightgrey);
    std::unique_ptr<juce::Drawable> drawableSettingsDownImage = Drawable::createFromSVG(*(Settings_svg_xml.get()));
    drawableSettingsDownImage->replaceColour(Colours::black, Colours::grey);

    m_openConfig->setImages(drawableSettingsNormalImage.get(), drawableSettingsOverImage.get(), drawableSettingsDownImage.get(), nullptr, nullptr, nullptr, nullptr, nullptr);
    addAndMakeVisible(m_openConfig.get());
    m_openConfig->addListener(this);

    m_openConfig->setVisible(false);
}

AbstractAudioVisualizer::~AbstractAudioVisualizer()
{
}


void AbstractAudioVisualizer::showConfigButton(bool enable)
{
    if(enable)
        m_openConfig->setVisible(true);
    else
        m_openConfig->setVisible(false);
}

void AbstractAudioVisualizer::paint (Graphics& g)
{
    Component::paint(g);
}

void AbstractAudioVisualizer::resized()
{
    auto buttonMargin = 1;
    auto buttonSize = 19;
    m_openConfig->setBounds(juce::Rectangle<int>(buttonMargin, buttonMargin, buttonSize, buttonSize));

    if (m_visualizerConfig)
    {
        m_visualizerConfig->setBounds(juce::Rectangle<int>(buttonMargin + buttonSize, buttonMargin, getWidth() - 2*buttonMargin - buttonSize, getHeight() - 2 * buttonMargin));
    }
}

void AbstractAudioVisualizer::mouseDown(const MouseEvent& event)
{
    // if visualizer config currently is opened and a click is performed outside of it, close it
    if (m_visualizerConfig && (event.eventComponent->getComponentID() != String(VISUALIZER_CONFIG_OPEN_ID)) && !m_visualizerConfig->getBounds().contains(event.getEventRelativeTo(this).getMouseDownPosition()))
        closeAudioVisualizerConfig();

    Component::mouseDown(event);
}

void AbstractAudioVisualizer::buttonClicked(Button* button)
{
    if (m_openConfig && m_openConfig.get() == button)
    {
        onOpenConfigClicked();
    }
}

void AbstractAudioVisualizer::onOpenConfigClicked()
{
    if (m_visualizerConfig)
        closeAudioVisualizerConfig();
    else
    {
        m_visualizerConfig = openAudioVisualizerConfig();
        addAndMakeVisible(m_visualizerConfig.get());
        resized();
    }
}

std::unique_ptr<AudioVisualizerConfigBase> AbstractAudioVisualizer::openAudioVisualizerConfig()
{
    return std::make_unique<AudioVisualizerConfigBase>(m_channelMapping);
}

void AbstractAudioVisualizer::closeAudioVisualizerConfig()
{
    if (m_visualizerConfig)
    {
        m_channelMapping = m_visualizerConfig->getChannelMapping();
        processChangedChannelMapping();
        m_visualizerConfig.reset();
    }
}

std::string AbstractAudioVisualizer::VisuTypeToString(VisuType type)
{
    switch(type)
    {
        case MultiMeter:
            return "MultiMeter";
        case Scope:
            return "Scope";
        case TwoDField:
            return "TwoDField";
        case Rta:
            return "RTA";
        case Waterfall:
            return "Waterfall";
        case Waveform:
            return "Waveform";
        case InvalidFirst:
        case InvalidLast:
        default:
            return "Invalid";
    }
}
