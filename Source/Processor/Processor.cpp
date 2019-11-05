/*
  ==============================================================================

    Processor.cpp
    Created: 1 Nov 2019 11:26:44am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Processor.h"

//==============================================================================
Processor::Processor()
    : AudioProcessor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Processor::~Processor()
{
}

void Processor::AddListener(Listener *listener)
{
    m_callbackListeners.add(listener);
}

void Processor::RemoveListener(Listener *listener)
{
    m_callbackListeners.remove(m_callbackListeners.indexOf(listener));
}

//==============================================================================
const String Processor::getName() const
{
    return m_Name;
}

void Processor::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void Processor::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void Processor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    /*dbg*/ignoreUnused(buffer);
    /*dbg*/ignoreUnused(midiMessages);
}

double Processor::getTailLengthSeconds() const
{
    /*dbg*/return 0.0;
}

bool Processor::acceptsMidi() const
{
    return false;
}

bool Processor::producesMidi() const
{
    return false;
}

AudioProcessorEditor* Processor::createEditor()
{
    /*dbg*/return nullptr;
}

bool Processor::hasEditor() const
{
    /*dbg*/return false;
}

int Processor::getNumPrograms()
{
    /*dbg*/return 0;
}

int Processor::getCurrentProgram()
{
    /*dbg*/return 0;
}

void Processor::setCurrentProgram (int index)
{
    /*dbg*/ignoreUnused(index);
}

const String Processor::getProgramName (int index)
{
    /*dbg*/ignoreUnused(index);
    /*dbg*/return String();
}

void Processor::changeProgramName (int index, const String& newName)
{
    /*dbg*/ignoreUnused(index);
    /*dbg*/ignoreUnused(newName);
}

void Processor::getStateInformation (juce::MemoryBlock& destData)
{
    /*dbg*/ignoreUnused(destData);
}

void Processor::setStateInformation (const void* data, int sizeInBytes)
{
    /*dbg*/ignoreUnused(data);
    /*dbg*/ignoreUnused(sizeInBytes);
}

void Processor::timerCallback()
{
    
}
