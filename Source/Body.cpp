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

#include "AudioVisualizer/TwoDFieldAudioVisualizer.h"
#include "AudioVisualizer/MultiMeterAudioVisualizer.h"
#include "AudioVisualizer/ScopeAudioVisualizer.h"
#include "AudioVisualizer/RtaAudioVisualizer.h"
#include "AudioVisualizer/WaterfallAudioVisualizer.h"
#include "AudioVisualizer/WaveformAudioVisualizer.h"

//==============================================================================
Body::Body()
{
	m_processor = 0;

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
	auto isPortrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();
    int visuCount = int(m_AudioVisualizers.size());
    //int rowCount = std::max(int(0.5f * visuCount + 0.5f) + int(isPortrait ? visuCount % 2 : 0), 1);
    //int colCount = std::max(int(0.5f * visuCount + 0.5f) + int(isPortrait ? 0 : visuCount % 2), 1);
	int rowCount = int(0.5f * visuCount + 0.5f);
	int colCount = int(0.5f * visuCount + 0.5f);
    
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

	//FlexBox fb;
	//fb.flexDirection = FlexBox::Direction::column;
	//fb.flexWrap = FlexBox::Wrap::wrap;
	//fb.justifyContent = FlexBox::JustifyContent::center;
	//fb.alignContent = FlexBox::AlignContent::center;
	//for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<AbstractAudioVisualizer>> &p : m_AudioVisualizers)
	//{
	//	if(p.second)
	//		fb.items.add(FlexItem(*p.second).withFlex(1));
	//}
	//fb.performLayout(getLocalBounds().toFloat());
}

void Body::setProcessor(Processor *processor)
{
    m_processor = processor;

	std::set<AbstractAudioVisualizer::VisuType> defaultVisuTypes{ AbstractAudioVisualizer::VisuType::MultiMeter };
	onUpdateVisuTypes(defaultVisuTypes);
}

void Body::onUpdateVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes)
{
    std::set<AbstractAudioVisualizer::VisuType> visuTypesToAdd, visuTypesToRemove;
    
    // clean up no longer used visualizers
    for(const std::pair<const AbstractAudioVisualizer::VisuType, std::unique_ptr<AbstractAudioVisualizer>> &p : m_AudioVisualizers)
    {
        if(visuTypes.count(p.first)==0)
            visuTypesToRemove.insert(p.first);
    }
    
    // create newly requested visualizers
    for(AbstractAudioVisualizer::VisuType type : visuTypes)
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
					// intentionally no break to run into default
                default:
                    m_AudioVisualizers[type] = std::make_unique<ScopeAudioVisualizer>();
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
