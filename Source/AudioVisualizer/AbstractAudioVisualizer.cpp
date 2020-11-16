/*
  ==============================================================================

    AbstractAudioVisualizer.cpp
    Created: 1 Nov 2019 11:39:06am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "AbstractAudioVisualizer.h"

#include "../submodules/JUCE-AppBasics/Source/Image_utils.hpp"

namespace Auvi
{

//==============================================================================
AudioVisualizerConfigBase::AudioVisualizerConfigBase()
{
    m_visualizerMappingSelects = std::map<AudioVisualizerConfigBase::MappingKey, std::unique_ptr<ComboBox>>{};
    m_visualizerMappingLabels = std::map<AudioVisualizerConfigBase::MappingKey, std::unique_ptr<Label>>{};

    m_usesValuesInDBSplitButton = std::make_unique<JUCEAppBasics::SplitButtonComponent>();
    addAndMakeVisible(m_usesValuesInDBSplitButton.get());
    m_usesValuesInDBSplitButton->addButton("dB");
    m_usesValuesInDBSplitButton->addButton("lin");
    m_usesValuesInDBSplitButton->addListener(this);
}

AudioVisualizerConfigBase::~AudioVisualizerConfigBase()
{
}

void AudioVisualizerConfigBase::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void AudioVisualizerConfigBase::resized()
{
    auto itemHeight = 19;
    auto space = itemHeight / 4;
    Rectangle<int> r(proportionOfWidth(0.35f), space, proportionOfWidth(0.6f), getHeight());

    if (m_configFeatures & ConfigFeatures::ChannelMapping)
    {
        for (auto iter = m_visualizerMappingSelects.cbegin(); iter != m_visualizerMappingSelects.cend(); iter++)
        {
            auto rect = r.removeFromTop(itemHeight);
            if (m_visualizerMappingLabels.count(iter->first) != 0)
                m_visualizerMappingLabels.at(iter->first)->setBounds(rect.withX(0).withWidth(proportionOfWidth(0.6f)));
            if (m_visualizerMappingSelects.count(iter->first) != 0)
                m_visualizerMappingSelects.at(iter->first)->setBounds(rect.withX(proportionOfWidth(0.6f)).withWidth(proportionOfWidth(0.4f) - space));
            r.removeFromTop(space);
        }
    }

    if (m_configFeatures & ConfigFeatures::UseValuesInDBToogle)
    {
        auto rect = r.removeFromTop(itemHeight);
        m_usesValuesInDBSplitButton->setBounds(rect.withX(proportionOfWidth(0.6f)).withWidth(proportionOfWidth(0.4f) - space));
        r.removeFromTop(space);
    }

    setSize(getWidth(), r.getY());
}

void AudioVisualizerConfigBase::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    for (auto iter = m_visualizerMappingSelects.cbegin(); iter != m_visualizerMappingSelects.cend(); iter++)
    {
        m_visualizerChannelMapping.at(iter->first) = iter->second->getSelectedId();
    }
}

void AudioVisualizerConfigBase::buttonClicked(JUCEAppBasics::SplitButtonComponent* button, uint64 buttonId)
{
    if (m_usesValuesInDBSplitButton && m_usesValuesInDBSplitButton.get() == button)
        setUsesValuesInDB(m_usesValuesInDBSplitButton->getButtonDownText() == "dB");
}

void AudioVisualizerConfigBase::setConfigFeatures(int features)
{
    m_configFeatures = features;
}

void AudioVisualizerConfigBase::setChannelMapping(std::map<AudioVisualizerConfigBase::MappingKey, int> mapping)
{
    m_visualizerChannelMapping = mapping;

    m_visualizerMappingLabels.clear();
    m_visualizerMappingSelects.clear();
    for (auto NameChannelKV : mapping)
    {
        m_visualizerMappingLabels.insert(std::make_pair(NameChannelKV.first, std::make_unique<Label>()));
        m_visualizerMappingLabels.at(NameChannelKV.first)->setText(this->getMappingString(NameChannelKV.first), dontSendNotification);
        addAndMakeVisible(m_visualizerMappingLabels.at(NameChannelKV.first).get());
        m_visualizerMappingSelects.insert(std::make_pair(NameChannelKV.first, std::make_unique<ComboBox>()));
        addAndMakeVisible(m_visualizerMappingSelects.at(NameChannelKV.first).get());
        m_visualizerMappingSelects.at(NameChannelKV.first)->addItemList({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" }, 1);
        m_visualizerMappingSelects.at(NameChannelKV.first)->setSelectedId(NameChannelKV.second, dontSendNotification);
        m_visualizerMappingSelects.at(NameChannelKV.first)->addListener(this);
    }
}

std::map<AudioVisualizerConfigBase::MappingKey, int> const& AudioVisualizerConfigBase::getChannelMapping()
{
    return m_visualizerChannelMapping;
}

void AudioVisualizerConfigBase::setUsesValuesInDB(bool usesValuesInDB)
{
    m_usesValuesInDB = usesValuesInDB;
    m_usesValuesInDBSplitButton->setButtonDown(usesValuesInDB ? "dB" : "lin");
}

bool AudioVisualizerConfigBase::getUsesValuesInDB()
{
    return m_usesValuesInDB;
}

//==============================================================================
AbstractAudioVisualizer::AbstractAudioVisualizer()
{
    m_changesPending = false;

    m_channelMapping = std::map<AudioVisualizerConfigBase::MappingKey, int>{};

    m_openConfig = std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageStretched);
    m_openConfig->setComponentID(VISUALIZER_CONFIG_OPEN_ID);
    m_openConfig->setClickingTogglesState(true);
    std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
    JUCEAppBasics::Image_utils::getDrawableButtonImages(BinaryData::settings24px_svg, NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);
    m_openConfig->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
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
        m_visualizerConfig->setChannelMapping(m_channelMapping);
        m_visualizerConfig->setUsesValuesInDB(m_usesValuesInDB);
        m_visualizerConfig->setConfigFeatures(static_cast<AudioVisualizerConfigBase::ConfigFeatures>(m_configFeatures));
        addAndMakeVisible(m_visualizerConfig.get());
        resized();
    }
}

std::unique_ptr<AudioVisualizerConfigBase> AbstractAudioVisualizer::openAudioVisualizerConfig()
{
    return std::make_unique<AudioVisualizerConfigBase>();
}

void AbstractAudioVisualizer::closeAudioVisualizerConfig()
{
    if (m_visualizerConfig)
    {
        m_channelMapping = m_visualizerConfig->getChannelMapping();
        processChangedChannelMapping();
        m_usesValuesInDB = m_visualizerConfig->getUsesValuesInDB();
        m_visualizerConfig.reset();

        triggerConfigurationUpdate(true);
    }
}


void AbstractAudioVisualizer::setConfigFeatures(int features)
{
    m_configFeatures = features;
}

int AbstractAudioVisualizer::getConfigFeatures()
{
    return m_configFeatures;
}

void AbstractAudioVisualizer::setChannelMapping(std::map<AudioVisualizerConfigBase::MappingKey, int> mapping)
{
    m_channelMapping = mapping;
}

std::map<AudioVisualizerConfigBase::MappingKey, int> const& AbstractAudioVisualizer::getChannelMapping()
{
    return m_channelMapping;
}

void AbstractAudioVisualizer::setUsesValuesInDB(bool useValuesInDB)
{
    m_usesValuesInDB = useValuesInDB;
}

bool AbstractAudioVisualizer::getUsesValuesInDB()
{
    return m_usesValuesInDB;
}

std::unique_ptr<XmlElement> AbstractAudioVisualizer::createStateXml()
{
    XmlElement visualizerElement(String(AbstractAudioVisualizer::VisuTypeToString(getType())));

    auto mappingStateXml = std::make_unique<XmlElement>(AppConfiguration::getTagName(AppConfiguration::TagID::VISUMAP));
    for (auto mapping : m_channelMapping)
    {
        XmlElement* mappingElement = mappingStateXml->createNewChildElement("M" + String(static_cast<int>(mapping.first)));
        if (mappingElement)
        {
            mappingElement->setAttribute("name", AudioVisualizerConfigBase::getMappingString(mapping.first));
            mappingElement->addTextElement(String(mapping.second));
        }
    }

    auto usesValuesInDBStateXml = std::make_unique<XmlElement>(AppConfiguration::getTagName(AppConfiguration::TagID::VISUUSEDB));
    usesValuesInDBStateXml->addTextElement(m_usesValuesInDB ? "true" : "false");

    visualizerElement.addChildElement(usesValuesInDBStateXml.release());
    visualizerElement.addChildElement(mappingStateXml.release());

    return std::make_unique<XmlElement>(visualizerElement);
}

bool AbstractAudioVisualizer::setStateXml(XmlElement* stateXml)
{
    if (!stateXml)
        return false;

    if (stateXml->getTagName() != String(AbstractAudioVisualizer::VisuTypeToString(getType())))
        return false;

    forEachXmlChildElement(*stateXml, stateXmlElement)
    {
        if (stateXmlElement->getTagName() == AppConfiguration::getTagName(AppConfiguration::TagID::VISUMAP))
        {
            forEachXmlChildElement(*stateXmlElement, mappingElement)
            {
                if (m_channelMapping.count(static_cast<AudioVisualizerConfigBase::MappingKey>(mappingElement->getTagName().getTrailingIntValue())) > 0)
                    m_channelMapping.at(static_cast<AudioVisualizerConfigBase::MappingKey>(mappingElement->getTagName().getTrailingIntValue())) = mappingElement->getAllSubText().getIntValue();
            }
        }
        else if (stateXmlElement->getTagName() == AppConfiguration::getTagName(AppConfiguration::TagID::VISUUSEDB))
        {
            auto valueString = stateXmlElement->getAllSubText();
            m_usesValuesInDB = (stateXmlElement->getAllSubText() == "true") ? true : false;
        }
    }

    processChangedChannelMapping();

    return true;
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

AbstractAudioVisualizer::VisuType AbstractAudioVisualizer::StringToVisuType(std::string typeName)
{
    if (typeName == "MultiMeter")
        return MultiMeter;
    else if (typeName == "Scope")
        return Scope;
    else if (typeName == "TwoDField")
        return TwoDField;
    else if (typeName == "RTA")
        return Rta;
    else if (typeName == "Waterfall")
        return Waterfall;
    else if (typeName == "Waveform")
        return Waveform;
    else
        return InvalidLast;
}

void AbstractAudioVisualizer::notifyChanges()
{
    m_changesPending = true;
    processChanges();
}

void AbstractAudioVisualizer::processChanges()
{
    if (m_changesPending)
    {
        m_changesPending = false;
        repaint();
    }
}

void AbstractAudioVisualizer::setPauseProcessing(bool pause)
{
    ignoreUnused(pause);
}

}
