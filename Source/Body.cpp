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
    std::set<AbstractAudioVisualizer::VisuType> defaultVisuTypes{AbstractAudioVisualizer::VisuType::MultiMeter};
    onUpdateVisuTypes(defaultVisuTypes);
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
    int visuCount = int(m_AudioVisualizers.size());
    int rowCount = 1;
    int colCount = 1;
    while(rowCount < colCount)
    {
        colCount = visuCount / rowCount;
    }
    
	Grid grid;
    using Track = Grid::TrackInfo;
    
    while(rowCount--)
        grid.templateRows.add(Track (1_fr));
    while(colCount--)
        grid.templateColumns.add(Track (1_fr));
    
    // clean up no longer used visualizers
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
        auto add = visuTypesToAdd.count((AbstractAudioVisualizer::VisuType)i)!=0;
        auto remove = visuTypesToRemove.count((AbstractAudioVisualizer::VisuType)i)!=0;
        
        if(add && remove)
        {
            jassertfalse;
            continue;
        }
        
        if(remove && m_AudioVisualizers.count((AbstractAudioVisualizer::VisuType)i))
        {
           if(m_processor)
               m_processor->removeListener(m_AudioVisualizers.at((AbstractAudioVisualizer::VisuType)i).get());
            m_AudioVisualizers.at((AbstractAudioVisualizer::VisuType)i).reset();
        }
        else if(add)
        {
            switch(i)
            {
                case AbstractAudioVisualizer::VisuType::MultiMeter:
                    m_AudioVisualizers[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<MultiMeterAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::TwoDField:
                    m_AudioVisualizers[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<TwoDFieldAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::Rta:
                    m_AudioVisualizers[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<RtaAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::Waterfall:
                    m_AudioVisualizers[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<WaterfallAudioVisualizer>();
                    break;
                case AbstractAudioVisualizer::VisuType::Waveform:
                    m_AudioVisualizers[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<WaveformAudioVisualizer>();
                    break;
                default:
                    m_AudioVisualizers[(AbstractAudioVisualizer::VisuType)i] = std::make_unique<ScopeAudioVisualizer>();
                    break;
            }
            addAndMakeVisible(m_AudioVisualizers.at((AbstractAudioVisualizer::VisuType)i).get());
           
            if(m_processor)
                m_processor->addListener(m_AudioVisualizers.at((AbstractAudioVisualizer::VisuType)i).get());
        }
    }
    
    resized();
}
