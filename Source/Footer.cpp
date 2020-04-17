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

	m_visuConfigOpen = std::make_unique<TextButton>();
    m_visuConfigOpen->setComponentID(VISU_CONFIG_OPEN_ID);
	m_visuConfigOpen->setButtonText("Visu Configuration");
    m_visuConfigOpen->setColour(TextButton::ColourIds::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
    m_visuConfigOpen->setColour(TextButton::ColourIds::buttonOnColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
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
    auto parentCpnt = getParentComponent();
    auto parentLstnr = dynamic_cast<Listener*>(parentCpnt);
    if (parentCpnt && parentLstnr)
	{
        auto topLeft = std::pair<int, int>{};
        auto size = std::pair<int, int>{};
        if (isVerticalButton)
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop);
            size = std::make_pair(parentCpnt->getWidth() - (2 * margin) - buttonHeight - m_noGoAreaLeft - m_noGoAreaRight,
                parentCpnt->getHeight() - (2 * margin) - m_noGoAreaTop - m_noGoAreaBottom);
        }
        else
        {
            topLeft = std::make_pair(margin + m_noGoAreaLeft,
                                     margin + m_noGoAreaTop);
            size = std::make_pair(parentCpnt->getWidth() - (2 * margin) - m_noGoAreaLeft - m_noGoAreaRight,
                parentCpnt->getHeight() - (2 * margin) - buttonHeight - m_noGoAreaTop - m_noGoAreaBottom);
        }

        if(parentLstnr && parentLstnr->getVisuConfigSelect())
            parentLstnr->getVisuConfigSelect()->setBounds(Rectangle<int>(topLeft.first, topLeft.second, size.first, size.second));
	}
}

void Footer::buttonClicked(Button* button)
{
	if (m_visuConfigOpen && m_visuConfigOpen.get() == button)
	{
		auto parentListener = dynamic_cast<Listener*>(getParentComponent());
		if (parentListener)
		{
            auto vsc = parentListener->getVisuConfigSelect();
			if (vsc)
                vsc->setVisible(false);
            vsc = parentListener->onOpenVisuConfigSelect();
			if (vsc)
                vsc->setVisible(true);

			resized();
		}
	}
}
