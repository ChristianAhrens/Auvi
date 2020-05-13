/*
  ==============================================================================

    Body.cpp
    Created: 20 Nov 2019 6:32:16pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "Body.h"

#include <set>

#include "../JuceLibraryCode/JuceHeader.h"

#include "AppConfiguration.h"

#include "AudioVisualizer/TwoDFieldAudioVisualizer.h"
#include "AudioVisualizer/MultiMeterAudioVisualizer.h"
#include "AudioVisualizer/ScopeAudioVisualizer.h"
#include "AudioVisualizer/RtaAudioVisualizer.h"
#include "AudioVisualizer/WaterfallAudioVisualizer.h"
#include "AudioVisualizer/WaveformAudioVisualizer.h"

namespace Auvi
{

//==============================================================================
Body::Body()
{
	m_processor = 0;
    m_isPortrait = true;
}

Body::~Body()
{
}

void Body::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void Body::resized()
{
    int rowCount = 1;
    int colCount = 1;
    
    int visuCount = int(m_AudioVisualizers.size());
    
    if(visuCount == 2)
    {
        rowCount = m_isPortrait ? 2 : 1;
        colCount = m_isPortrait ? 1 : 2;
    }
    else if(visuCount != 1)
    {
        int halfVisuCount = int(0.5f * visuCount + 0.5f);
        
        rowCount = m_isPortrait ? halfVisuCount : 2;
        colCount = m_isPortrait ? 2 : halfVisuCount;
    }
    
	Grid grid;
    using Track = Grid::TrackInfo;
    
    while(rowCount--)
        grid.templateRows.add(Track (1_fr));
    while(colCount--)
        grid.templateColumns.add(Track (1_fr));
    
    for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<AbstractAudioVisualizer>> &p : m_AudioVisualizers)
    {
        if(p.second)
            grid.items.add(*p.second);
    }
    
    grid.performLayout (getLocalBounds());
}

void Body::setProcessor(Processor *processor)
{
    m_processor = processor;
}

void Body::setPortrait(bool portrait)
{
    m_isPortrait = portrait;
}

void Body::onUpdateVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes)
{
    std::set<AbstractAudioVisualizer::VisuType> visuTypesToAdd, visuTypesToRemove;
    
    // clean up no longer used visualizers
    for(auto &p : m_AudioVisualizers)
    {
        if(visuTypes.count(p.first)==0)
            visuTypesToRemove.insert(p.first);
    }
    
    // create newly requested visualizers
    for(auto type : visuTypes)
    {
        if(m_AudioVisualizers.count(type)==0)
            visuTypesToAdd.insert(type);
    }
    
    
    for(int i=AbstractAudioVisualizer::VisuType::InvalidFirst+1; i<AbstractAudioVisualizer::VisuType::InvalidLast; ++i)
    {
		AbstractAudioVisualizer::VisuType type = (AbstractAudioVisualizer::VisuType)i;

        auto add = visuTypesToAdd.count(type)!=0;
        auto remove = visuTypesToRemove.count(type)!=0;
        
        if(add && remove)
        {
            jassertfalse;
            continue;
        }
        
        if(remove && m_AudioVisualizers.count(type))
        {
           if(m_processor)
               m_processor->removeListener(m_AudioVisualizers.at(type).get());
            m_AudioVisualizers.at(type).reset();
			m_AudioVisualizers.erase(type);
        }
        else if(add)
        {
            switch(type)
            {
                case AbstractAudioVisualizer::VisuType::MultiMeter:
                    m_AudioVisualizers[type] = std::make_unique<MultiMeterAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::TwoDField:
                    m_AudioVisualizers[type] = std::make_unique<TwoDFieldAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::Rta:
                    m_AudioVisualizers[type] = std::make_unique<RtaAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::Waterfall:
                    m_AudioVisualizers[type] = std::make_unique<WaterfallAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::Waveform:
                    m_AudioVisualizers[type] = std::make_unique<WaveformAudioVisualizer>();
                    break;
				case AbstractAudioVisualizer::VisuType::Scope:
                    m_AudioVisualizers[type] = std::make_unique<ScopeAudioVisualizer>();
                    break;
                default:
                    break;
            }
            addAndMakeVisible(m_AudioVisualizers.at(type).get());
           
            if(m_processor)
                m_processor->addListener(m_AudioVisualizers.at(type).get());
        }
    }
    
    resized();
}

const std::set<AbstractAudioVisualizer::VisuType> Body::getActiveVisuTypes()
{
	std::set<AbstractAudioVisualizer::VisuType> activeVisuTypes;

	for (const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<AbstractAudioVisualizer>>& p : m_AudioVisualizers)
	{
		activeVisuTypes.insert(p.first);
	}

	return activeVisuTypes;
}

std::unique_ptr<XmlElement> Body::createVisuStateXml()
{
    auto activeVisuTypes = getActiveVisuTypes();

    auto activeVisualizersElement = std::make_unique<XmlElement>(AppConfiguration::getTagName(AppConfiguration::TagID::VISU));
    for (auto i = AbstractAudioVisualizer::InvalidFirst + 1; i < AbstractAudioVisualizer::InvalidLast; ++i)
    {
        AbstractAudioVisualizer::VisuType visuType = static_cast<AbstractAudioVisualizer::VisuType>(i);
        XmlElement* visualizerTypeElement = activeVisualizersElement->createNewChildElement(AbstractAudioVisualizer::VisuTypeToString(visuType));
        if(visualizerTypeElement)
        {
            visualizerTypeElement->setAttribute("isActive", (activeVisuTypes.count(visuType) > 0) ? 1 : 0);

            if (m_AudioVisualizers.count(visuType) > 0)
            {
                auto mappingStateXml = m_AudioVisualizers.at(visuType)->createStateXml();
                visualizerTypeElement->addChildElement(mappingStateXml.release());
            }
        }
    }

    return std::make_unique<XmlElement>(*activeVisualizersElement);
}

bool Body::setVisuStateXml(XmlElement* stateXml)
{
    if (!stateXml || (stateXml->getTagName() != AppConfiguration::getTagName(AppConfiguration::TagID::VISU)))
        return false;

    std::set<AbstractAudioVisualizer::VisuType> visualizerTypes = {};
    std::map< AbstractAudioVisualizer::VisuType, XmlElement*> visualizerXmlElements;
    forEachXmlChildElement(*stateXml, visualizerChildElement)
    {
        auto visuType = AbstractAudioVisualizer::StringToVisuType(visualizerChildElement->getTagName().toStdString());
        auto isActiveAttributeValue = visualizerChildElement->getBoolAttribute("isActive");
        if (visuType < AbstractAudioVisualizer::InvalidLast && isActiveAttributeValue)
        {
            visualizerTypes.insert(visuType);
            visualizerXmlElements.insert(std::make_pair(visuType, visualizerChildElement->getChildByName(AppConfiguration::getTagName(AppConfiguration::TagID::VISUMAP))));
        }
    }

    onUpdateVisuTypes(visualizerTypes);

    for (auto mappingStateXml : visualizerXmlElements)
    {
        if (m_AudioVisualizers.count(mappingStateXml.first) > 0)
            m_AudioVisualizers.at(mappingStateXml.first)->setStateXml(mappingStateXml.second);
    }

    return true;
}

}
