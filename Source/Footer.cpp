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
Footer::Footer(int noGoAreaBottom, int noGoAreaLeft)
{
    setNoGoArea(noGoAreaBottom, noGoAreaLeft);
    m_visuConfigSelect = nullptr;

	m_visuConfigOpen = std::make_unique<TextButton>();
	m_visuConfigOpen->setButtonText("Visu Configuration");
	addAndMakeVisible(m_visuConfigOpen.get());
	m_visuConfigOpen->addListener(this);
}

Footer::~Footer()
{
}

void Footer::setNoGoArea(int noGoAreaBottom, int noGoAreaLeft)
{
    m_noGoAreaBottom = noGoAreaBottom;
    m_noGoAreaLeft = noGoAreaLeft;
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

	auto isVertical = getWidth() < (buttonWidth + margin + margin);
    
    m_visuConfigOpen->setSize(buttonWidth, buttonHeight);
    
    if (isVertical)
        m_visuConfigOpen->setTransform(AffineTransform::rotation(0.5f * float_Pi).translated(float(getWidth() - (margin + m_noGoAreaLeft)), margin));
    else
        m_visuConfigOpen->setTransform(AffineTransform::rotation(0).translated(margin + m_noGoAreaLeft, margin));

	Component* pc = getParentComponent();
	if (m_visuConfigSelect && pc)
	{
        if (isVertical)
            m_visuConfigSelect->setBounds(Rectangle<int>(margin, margin, pc->getWidth() - 2 * margin - buttonHeight, pc->getHeight() - 2 * margin));
        else
            m_visuConfigSelect->setBounds(Rectangle<int>(margin, margin, pc->getWidth() - 2 * margin, pc->getHeight() - 2 * margin - buttonHeight));
	}
}

void Footer::buttonClicked(Button* button)
{
	if (m_visuConfigOpen && m_visuConfigOpen.get() == button)
	{
		Listener* acListener = dynamic_cast<Listener*>(getParentComponent());
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
