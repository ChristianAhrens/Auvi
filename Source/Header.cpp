/*
  ==============================================================================

    Header.cpp
    Created: 1 Nov 2019 11:34:46am
    Author:  Christian Ahrens

  ==============================================================================
*/
#include "Header.h"

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
Header::Header(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight)
{
    setNoGoArea(noGoAreaTop, noGoAreaBottom, noGoAreaLeft, noGoAreaRight);
	m_audioConfigSelect = nullptr;

	m_audioConfigOpen = std::make_unique<TextButton>();
	m_audioConfigOpen->setButtonText("Audio Configuration");
	addAndMakeVisible(m_audioConfigOpen.get());
	m_audioConfigOpen->addListener(this);

    m_stopProcessing = std::make_unique<DrawableButton>(String(), DrawableButton::ButtonStyle::ImageOnButtonBackground);
    m_stopProcessing->setClickingTogglesState(true);
    std::unique_ptr<XmlElement> Pause_svg_xml = XmlDocument::parse(BinaryData::pause24px_svg);
    std::unique_ptr<juce::Drawable> drawablePauseImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawablePauseImage->replaceColour(Colours::black, Colours::white);
    std::unique_ptr<juce::Drawable> drawableOnImage = Drawable::createFromSVG(*(Pause_svg_xml.get()));
    drawableOnImage->replaceColour(Colours::black, Colours::red);
    m_stopProcessing->setImages(drawablePauseImage.get(), nullptr, nullptr, nullptr, drawableOnImage.get(), nullptr, nullptr, nullptr);
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
                                     margin + m_noGoAreaTop + margin + buttonWidth);
    }
    else
    {
        translation = std::make_pair(margin + m_noGoAreaLeft + buttonWidth + margin,
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
    auto pc = getParentComponent();
    if (m_audioConfigSelect && pc)
    {
        auto topLeft = std::pair<int, int>{};
        auto size = std::pair<int, int>{};
        if (isVerticalButton)
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft + buttonHeight,
                                     margin + m_noGoAreaTop);
            size = std::make_pair(pc->getWidth() - (2 * margin) - buttonHeight - m_noGoAreaLeft - m_noGoAreaRight,
                                  pc->getHeight() - (2 * margin) - m_noGoAreaTop - m_noGoAreaBottom);
        }
        else
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop + buttonHeight);
            size = std::make_pair(pc->getWidth() - (2 * margin) - m_noGoAreaLeft - m_noGoAreaRight,
                                  pc->getHeight() - (2 * margin) - buttonHeight - m_noGoAreaTop - m_noGoAreaBottom);
        }
        m_audioConfigSelect->setBounds(Rectangle<int>(topLeft.first, topLeft.second, size.first, size.second));
    }
}

void Header::buttonClicked(Button* button)
{
	if (m_audioConfigOpen && m_audioConfigOpen.get() == button)
	{
		auto acListener = dynamic_cast<Listener*>(getParentComponent());
		if (acListener)
		{
			if (m_audioConfigSelect)
				m_audioConfigSelect->setVisible(false);
			m_audioConfigSelect = acListener->onOpenAudioConfig();
			if (m_audioConfigSelect)
				m_audioConfigSelect->setVisible(true);

			resized();
		}
	}
    else if (m_stopProcessing && m_stopProcessing.get() == button)
    {
        auto acListener = dynamic_cast<Listener*>(getParentComponent());
        if (acListener)
        {
            acListener->onPauseProcessing(m_stopProcessing->getToggleState());
        }
    }
}
