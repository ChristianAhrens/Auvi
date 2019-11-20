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
Footer::Footer()
{
    m_visuConfigSelect = nullptr;

	m_visuConfigOpen = std::make_unique<TextButton>();
	m_visuConfigOpen->setButtonText("Visu Configuration");
	addAndMakeVisible(m_visuConfigOpen.get());
	m_visuConfigOpen->addListener(this);
}

Footer::~Footer()
{
}

void Footer::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());
}

void Footer::resized()
{
    auto isVertical = getWidth() < (110 + 10 + 10);
    
    auto buttonWidth = 110;
    auto buttonHeight = 20;
    
    m_visuConfigOpen->setSize(buttonWidth, buttonHeight);
    
    if (isVertical)
        m_visuConfigOpen->setTransform(AffineTransform::rotation(0.5f * float_Pi).translated(float(getWidth() - 10), 10.0f));
    else
        m_visuConfigOpen->setTransform(AffineTransform::rotation(0).translated(10.0f, 10.0f));

	Component* pc = getParentComponent();
	if (m_visuConfigSelect && pc)
	{
        if (isVertical)
            m_visuConfigSelect->setBounds(Rectangle<int>(10, 10, pc->getWidth() - 2 * 10 - buttonHeight, pc->getHeight() - 2 * 10));
        else
            m_visuConfigSelect->setBounds(Rectangle<int>(10, 10, pc->getWidth() - 2 * 10, pc->getHeight() - 2 * 10 - buttonHeight));
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
