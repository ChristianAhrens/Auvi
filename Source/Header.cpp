/*
  ==============================================================================

    Header.cpp
    Created: 1 Nov 2019 11:34:46am
    Author:  Christian Ahrens

  ==============================================================================
*/
#include "Header.h"

#include "../JuceLibraryCode/JuceHeader.h"

namespace Auvi
{

//==============================================================================
Header::Header(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight)
{
    setNoGoArea(noGoAreaTop, noGoAreaBottom, noGoAreaLeft, noGoAreaRight);

	m_audioConfigOpen = std::make_unique<TextButton>();
    m_audioConfigOpen->setComponentID(AUDIO_CONFIG_OPEN_ID);
	m_audioConfigOpen->setButtonText("Audio Configuration");
    m_audioConfigOpen->setColour(TextButton::ColourIds::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    m_audioConfigOpen->setColour(TextButton::ColourIds::buttonOnColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
	addAndMakeVisible(m_audioConfigOpen.get());
	m_audioConfigOpen->addListener(this);

    m_stopProcessing = std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageStretched);
    m_stopProcessing->setComponentID(STOP_PROCESSING_ID);
    m_stopProcessing->setClickingTogglesState(true);
    std::unique_ptr<XmlElement> Pause_svg_xml = XmlDocument::parse(BinaryData::pause24px_svg);
    // create svg images from resources for regular state
    std::unique_ptr<juce::Drawable> drawablePauseNormalImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseNormalImage->replaceColour(Colours::black, Colours::white);
    std::unique_ptr<juce::Drawable> drawablePauseOverImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseOverImage->replaceColour(Colours::black, Colours::lightgrey);
    std::unique_ptr<juce::Drawable> drawablePauseDownImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseDownImage->replaceColour(Colours::black, Colours::grey);
    // create svg images from resources for ON state
    std::unique_ptr<juce::Drawable> drawablePauseNormalOnImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseNormalOnImage->replaceColour(Colours::black, Colours::red);
    std::unique_ptr<juce::Drawable> drawablePauseOverOnImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseOverOnImage->replaceColour(Colours::black, Colours::indianred);
    std::unique_ptr<juce::Drawable> drawablePauseDownOnImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseDownOnImage->replaceColour(Colours::black, Colours::darkred);
    // set the images to button
    m_stopProcessing->setImages(drawablePauseNormalImage.get(), drawablePauseOverImage.get(), drawablePauseDownImage.get(), nullptr, drawablePauseNormalOnImage.get(), drawablePauseOverOnImage.get(), drawablePauseDownOnImage.get(), nullptr);
    m_stopProcessing->setColour(DrawableButton::ColourIds::backgroundColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    m_stopProcessing->setColour(DrawableButton::ColourIds::backgroundOnColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    addAndMakeVisible(m_stopProcessing.get());
    m_stopProcessing->addListener(this);
}

Header::~Header()
{
}

void Header::setNoGoArea(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight)
{
    m_noGoAreaTop = noGoAreaTop;
    m_noGoAreaBottom = noGoAreaBottom;
    m_noGoAreaLeft = noGoAreaLeft;
    m_noGoAreaRight = noGoAreaRight;
}

void Header::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void Header::resized()
{
	auto buttonWidth = 140;
	auto buttonHeight = 26;
	auto margin = 7;

	auto isVerticalButton = getWidth() < (buttonWidth + margin + margin);

	m_audioConfigOpen->setSize(buttonWidth, buttonHeight);
    m_stopProcessing->setSize(buttonHeight, buttonHeight);

    // rotate and position the button to pause processing
    auto rotation = 0.0f;
    auto translation = std::pair<int, int>{};
    if (isVerticalButton)
    {
        translation = std::make_pair(margin + m_noGoAreaLeft,
                                    getHeight() - margin - m_noGoAreaRight - buttonHeight);
    }
    else
    {
        translation = std::make_pair(getWidth() - margin - m_noGoAreaRight - buttonHeight,
                                     margin + m_noGoAreaTop);
    }
    m_stopProcessing->setTransform(AffineTransform::rotation(rotation).translated(translation.first, translation.second));

    // rotate and position the button to open config
    rotation = 0.0f;
    translation = std::pair<int, int>{};
    if (isVerticalButton)
    {
        rotation = 0.5f * float_Pi;
        translation = std::make_pair(getWidth() - margin,
                                     margin + m_noGoAreaTop);
    }
    else
    {
        rotation = 0.0f;
        translation = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop);
    }
    m_audioConfigOpen->setTransform(AffineTransform::rotation(rotation).translated(translation.first, translation.second));

    // position the config selection component itself
    auto parentCpnt = getParentComponent();
    auto parentLstnr = dynamic_cast<Listener*>(parentCpnt);
    if (parentCpnt && parentLstnr)
    {
        auto topLeft = std::pair<int, int>{};
        auto size = std::pair<int, int>{};
        if (isVerticalButton)
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft + buttonHeight,
                                     margin + m_noGoAreaTop);
            size = std::make_pair(parentCpnt->getWidth() - (2 * margin) - buttonHeight - m_noGoAreaLeft - m_noGoAreaRight,
                parentCpnt->getHeight() - (2 * margin) - m_noGoAreaTop - m_noGoAreaBottom);
        }
        else
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop + buttonHeight);
            size = std::make_pair(parentCpnt->getWidth() - (2 * margin) - m_noGoAreaLeft - m_noGoAreaRight,
                parentCpnt->getHeight() - (2 * margin) - buttonHeight - m_noGoAreaTop - m_noGoAreaBottom);
        }

        if(parentLstnr && parentLstnr->getAudioConfigSelect())
            parentLstnr->getAudioConfigSelect()->setBounds(Rectangle<int>(topLeft.first, topLeft.second, size.first, size.second));
    }
}

void Header::buttonClicked(Button* button)
{
	if (m_audioConfigOpen && m_audioConfigOpen.get() == button)
	{
		auto parentListener = dynamic_cast<Listener*>(getParentComponent());
		if (parentListener)
		{
            auto asc = parentListener->getAudioConfigSelect();
			if (asc)
                asc->setVisible(false);
			asc = parentListener->onOpenAudioConfigSelect();
			if (asc)
                asc->setVisible(true);

			resized();
		}
	}
    else if (m_stopProcessing && m_stopProcessing.get() == button)
    {
        auto parentListener = dynamic_cast<Listener*>(getParentComponent());
        if (parentListener)
        {
            parentListener->onPauseProcessing(m_stopProcessing->getToggleState());
        }
    }
}

}