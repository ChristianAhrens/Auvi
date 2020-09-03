/*
  ==============================================================================

    Body.h
    Created: 20 Nov 2019 6:32:16pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AudioVisualizer/AbstractAudioVisualizer.h"
#include "VisuSelectComponent.h"

namespace Auvi
{

//==============================================================================
/* Fwd declarations */
class Processor;

//==============================================================================
/*
*/
class Body :    public Component, 
                public VisuSelectComponent::Listener, 
                public AppConfiguration::XmlConfigurableElement
{
public:
    Body();
    ~Body();
    
    void setPause(bool pause);

    void paint (Graphics&) override;
    void resized() override;
    
    void setProcessor(Processor *processor);
    void setPortrait(bool portrait);

    std::unique_ptr<XmlElement> createStateXml() override;
    bool setStateXml(XmlElement* stateXml) override;
    
    //==============================================================================
    void onUpdateVisuTypes(std::set<AbstractAudioVisualizer::VisuType> visuTypes) override;
	const std::set<AbstractAudioVisualizer::VisuType> getActiveVisuTypes() override;

private:
    std::map<AbstractAudioVisualizer::VisuType, std::unique_ptr<AbstractAudioVisualizer>>    m_AudioVisualizers;
    Processor *m_processor;
    bool m_isPortrait;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Body)
};

}
