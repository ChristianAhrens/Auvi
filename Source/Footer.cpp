/*
  ==============================================================================

    Footer.cpp
    Created: 1 Nov 2019 11:35:51am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Footer.h"
#include "AudioVisualizer/AbstractAudioVisualizer.h"

//==============================================================================
Footer::Footer(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight)
{
    setNoGoArea(noGoAreaTop, noGoAreaBottom, noGoAreaLeft, noGoAreaRight);
    m_visuConfigSelect = nullptr;

	m_visuConfigOpen = std::make_unique<TextButton>();
	m_visuConfigOpen->setButtonText("Visu Configuration");
	addAndMakeVisible(m_visuConfigOpen.get());
	m_visuConfigOpen->addListener(this);
}

Footer::~Footer()
{
}

void Footer::setNoGoArea(int noGoAreaTop, int noGoAreaBottom, int noGoAreaLeft, int noGoAreaRight)
{
    m_noGoAreaTop = noGoAreaTop;
    m_noGoAreaBottom = noGoAreaBottom;
    m_noGoAreaLeft = noGoAreaLeft;
    m_noGoAreaRight = noGoAreaRight;
}

void Footer::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void Footer::resized()
{
    auto buttonWidth = 140;
    auto buttonHeight = 26;
	auto margin = 7;

	auto isVerticalButton = getWidth() < (buttonWidth + margin + margin);
    
    m_visuConfigOpen->setSize(buttonWidth, buttonHeight);
    
    // rotate and position the button to open config
    auto rotation = 0.0f;
    auto translation = std::pair<int, int>{};
    if (isVerticalButton)
    {
        rotation = 0.5f * float_Pi;
        translation = std::make_pair(getWidth() - margin - m_noGoAreaRight,
                                     margin + m_noGoAreaTop);
    }
    else
    {
        rotation = 0.0f;
        translation = std::make_pair(margin + m_noGoAreaLeft,
                                     margin);
    }
    m_visuConfigOpen->setTransform(AffineTransform::rotation(rotation).translated(translation.first, translation.second));

    // position the config selection component itself
	auto pc = getParentComponent();
	if (m_visuConfigSelect && pc)
	{
        auto topLeft = std::pair<int, int>{};
        auto size = std::pair<int, int>{};
        if (isVerticalButton)
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop);
            size = std::make_pair(pc->getWidth() - (2 * margin) - buttonHeight - m_noGoAreaLeft - m_noGoAreaRight,
                                  pc->getHeight() - (2 * margin) - m_noGoAreaTop - m_noGoAreaBottom);
        }
        else
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop);
            size = std::make_pair(pc->getWidth() - (2 * margin) - m_noGoAreaLeft - m_noGoAreaRight,
                                  pc->getHeight() - (2 * margin) - buttonHeight - m_noGoAreaTop - m_noGoAreaBottom);
        }
        m_visuConfigSelect->setBounds(Rectangle<int>(topLeft.first, topLeft.second, size.first, size.second));
	}
}

void Footer::buttonClicked(Button* button)
{
	if (m_visuConfigOpen && m_visuConfigOpen.get() == button)
	{
		auto acListener = dynamic_cast<Listener*>(getParentComponent());
		if (acListener)
		{
			if (m_visuConfigSelect)
				m_visuConfigSelect->setVisible(false);
			m_visuConfigSelect = acListener->onOpenVisuConfig();
			if (m_visuConfigSelect)
				m_visuConfigSelect->setVisible(true);

			resized();
		}
	}
}
