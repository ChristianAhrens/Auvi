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
Header::Header()
{
	m_audioConfigSelect = nullptr;

	m_audioConfigOpen = std::make_unique<TextButton>();
	m_audioConfigOpen->setButtonText("Configuration");
	addAndMakeVisible(m_audioConfigOpen.get());
	m_audioConfigOpen->addListener(this);
}

Header::~Header()
{
}

void Header::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void Header::resized()
{
	auto isVertical = getWidth() < (110 + 10 + 10);

	auto buttonWidth = 110;
	auto buttonHeight = 20;

	m_audioConfigOpen->setSize(buttonWidth, buttonHeight);

	if (isVertical)
		m_audioConfigOpen->setTransform(AffineTransform::rotation(0.5f * float_Pi).translated(float(getWidth() - 10), 10.0f));
	else
		m_audioConfigOpen->setTransform(AffineTransform::rotation(0).translated(10.0f, 10.0f));

	Component* pc = getParentComponent();
	if (m_audioConfigSelect && pc)
	{
		if (isVertical)
			m_audioConfigSelect->setBounds(Rectangle<int>(10 + buttonHeight, 10, pc->getWidth() - 2 * 10 - buttonHeight, pc->getHeight() - 2 * 10));
		else
			m_audioConfigSelect->setBounds(Rectangle<int>(10, 10 + buttonHeight, pc->getWidth() - 2 * 10, pc->getHeight() - 2 * 10 - buttonHeight));
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
