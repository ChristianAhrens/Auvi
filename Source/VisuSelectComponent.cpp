/*
  ==============================================================================

    VisuSelectComponent.cpp
    Created: 19 Nov 2019 7:33:56pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "VisuSelectComponent.h"

#include <Image_utils.h>

namespace Auvi
{

//==============================================================================
VisuSelectComponent::VisuSelectComponent()
	: Component()
{
    m_listener = 0;
    
    for(int i=AbstractAudioVisualizer::VisuType::InvalidFirst+1; i<AbstractAudioVisualizer::VisuType::InvalidLast; ++i)
    {
        m_visuSelectButtons[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<DrawableButton>(String(i), DrawableButton::ButtonStyle::ImageOnButtonBackground);
        
        std::unique_ptr<juce::Drawable> NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage;
        JUCEAppBasics::Image_utils::getDrawableButtonImages(getVisuTypeFileName((AbstractAudioVisualizer::VisuType)i), NormalImage, OverImage, DownImage, DisabledImage, NormalOnImage, OverOnImage, DownOnImage, DisabledOnImage);

        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->setImages(NormalImage.get(), OverImage.get(), DownImage.get(), DisabledImage.get(), NormalOnImage.get(), OverOnImage.get(), DownOnImage.get(), DisabledOnImage.get());
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->setColour(TextButton::ColourIds::buttonOnColourId, Colours::grey);
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->setClickingTogglesState(true);
        addAndMakeVisible(m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i).get());
        m_visuSelectButtons.at((AbstractAudioVisualizer::VisuType)i)->addListener(this);
    }
}

VisuSelectComponent::VisuSelectComponent(std::set<AbstractAudioVisualizer::VisuType> visuTypes)
	: VisuSelectComponent()
{
	setActiveVisuTypes(visuTypes);
}

VisuSelectComponent::~VisuSelectComponent()
{
    if(m_listener)
    {
        std::set<AbstractAudioVisualizer::VisuType> activeVisuTypes;
        for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<DrawableButton>> &p : m_visuSelectButtons)
        {
            if(p.second->getToggleState())
                activeVisuTypes.insert(p.first);
        }
        
        m_listener->onUpdateVisuTypes(activeVisuTypes);
    }
}

String VisuSelectComponent::getVisuTypeFileName(AbstractAudioVisualizer::VisuType type)
{
    switch(type)
    {
        case AbstractAudioVisualizer::VisuType::MultiMeter:
            return BinaryData::equalizer24px_svg;
        case AbstractAudioVisualizer::VisuType::Scope:
            return BinaryData::track_changes24px_svg;
        case AbstractAudioVisualizer::VisuType::Rta:
             return BinaryData::show_chart24px_svg;
        case AbstractAudioVisualizer::VisuType::Waveform:
            return BinaryData::graphic_eq24px_svg;
        case AbstractAudioVisualizer::VisuType::Waterfall:
            return BinaryData::waves24px_svg;
        case AbstractAudioVisualizer::VisuType::TwoDField:
            return BinaryData::grid_on24px_svg;
        case AbstractAudioVisualizer::VisuType::InvalidFirst:
        case AbstractAudioVisualizer::VisuType::InvalidLast:
        default:
            return String();
    }
}

void VisuSelectComponent::setActiveVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes)
{
	for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<DrawableButton>>& p : m_visuSelectButtons)
	{
		if(visuTypes.count(p.first) != 0)
			p.second->setToggleState(true, dontSendNotification);
	}
}

void VisuSelectComponent::addListener(Listener *l)
{
    m_listener = l;

	if (m_listener)
		setActiveVisuTypes(m_listener->getActiveVisuTypes());
}

void VisuSelectComponent::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	Component::paint(g);
}

void VisuSelectComponent::resized()
{
    Grid grid;
    using Track = Grid::TrackInfo;
    grid.templateRows    = { Track (1_fr), Track (1_fr) };
    grid.templateColumns = { Track (1_fr), Track (1_fr), Track (1_fr) };
    for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<DrawableButton>> &p : m_visuSelectButtons)
    {
        grid.items.add(GridItem(p.second.get()).withMargin(juce::GridItem::Margin(5, 5, 5, 5)));
    }
    grid.performLayout(getLocalBounds().reduced(5));
}

void VisuSelectComponent::buttonClicked(Button* button)
{
    if (/*m_audioConfigOpen && m_audioConfigOpen.get() ==*/ button)
    {
        if(m_listener)
        {
        }
    }
}

}
