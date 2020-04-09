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
AbstractAudioVisualizerConfig::AbstractAudioVisualizerConfig()
{
}

AbstractAudioVisualizerConfig::~AbstractAudioVisualizerConfig()
{
}

void AbstractAudioVisualizerConfig::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("AudioVisualizerConfig", getLocalBounds(),
        Justification::centred, true);   // draw some placeholder text
}

void AbstractAudioVisualizerConfig::resized()
{
}

void AbstractAudioVisualizerConfig::buttonClicked(Button* button)
{
}

//==============================================================================
AbstractAudioVisualizer::AbstractAudioVisualizer()
{
    m_openConfig = std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageOnButtonBackground);
    m_openConfig->setComponentID(VISUALIZER_CONFIG_OPEN_ID);
    m_openConfig->setClickingTogglesState(true);
    std::unique_ptr<XmlElement> Settings_svg_xml = XmlDocument::parse(BinaryData::settings24px_svg);
    std::unique_ptr<juce::Drawable> drawableSettingsNormalImage = Drawable::createFromSVG(*(Settings_svg_xml.get()));
    drawableSettingsNormalImage->replaceColour(Colours::black, Colours::white);
    m_openConfig->setImages(drawableSettingsNormalImage.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
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
        m_visualizerConfig.reset();
    else
    {
        m_visualizerConfig = openAudioVisualizerConfig();
        addAndMakeVisible(m_visualizerConfig.get());
        resized();
    }
}

std::unique_ptr<AbstractAudioVisualizerConfig> AbstractAudioVisualizer::openAudioVisualizerConfig()
{
    return std::make_unique<AbstractAudioVisualizerConfig>();
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
