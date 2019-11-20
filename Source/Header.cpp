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
	m_audioConfigOpen->setButtonText("Audio Configuration");
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
	auto buttonWidth = 140;
	auto buttonHeight = 26;
	auto margin = 7;

	auto isVertical = getWidth() < (buttonWidth + margin + margin);

	m_audioConfigOpen->setSize(buttonWidth, buttonHeight);

	if (isVertical)
		m_audioConfigOpen->setTransform(AffineTransform::rotation(0.5f * float_Pi).translated(float(getWidth() - margin), margin));
	else
		m_audioConfigOpen->setTransform(AffineTransform::rotation(0).translated(margin, margin));

	Component* pc = getParentComponent();
	if (m_audioConfigSelect && pc)
	{
		if (isVertical)
			m_audioConfigSelect->setBounds(Rectangle<int>(margin + buttonHeight, margin, pc->getWidth() - 2 * margin - buttonHeight, pc->getHeight() - 2 * margin));
		else
			m_audioConfigSelect->setBounds(Rectangle<int>(margin, margin + buttonHeight, pc->getWidth() - 2 * margin, pc->getHeight() - 2 * margin - buttonHeight));
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
