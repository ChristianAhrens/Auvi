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

    // rotate and position the button to open config
    auto rotation = 0.0f;
    auto translation = std::pair<int, int>{};
    if (isVerticalButton)
    {
        rotation = 0.5f * float_Pi;
        translation = std::make_pair(float(getWidth() - margin),
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
		Listener* acListener = dynamic_cast<Listener*>(getParentComponent());
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
}
