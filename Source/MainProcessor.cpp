/*
  ==============================================================================

    MainProcessor.cpp
    Created: 1 Nov 2019 11:26:44am
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainProcessor.h"

//==============================================================================
MainProcessor::MainProcessor()
    : AudioProcessor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MainProcessor::~MainProcessor()
{
}

//==============================================================================
const String MainProcessor::getName() const
{
    return m_Name;
}

void MainProcessor::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainProcessor::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void MainProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    /*dbg*/ignoreUnused(buffer);
    /*dbg*/ignoreUnused(midiMessages);
}

double MainProcessor::getTailLengthSeconds() const
{
    /*dbg*/return 0.0;
}

bool MainProcessor::acceptsMidi() const
{
    return false;
}

bool MainProcessor::producesMidi() const
{
    return false;
}

AudioProcessorEditor* MainProcessor::createEditor()
{
    /*dbg*/return nullptr;
}

bool MainProcessor::hasEditor() const
{
    /*dbg*/return false;
}

int MainProcessor::getNumPrograms()
{
    /*dbg*/return 0;
}

int MainProcessor::getCurrentProgram()
{
    /*dbg*/return 0;
}

void MainProcessor::setCurrentProgram (int index)
{
    /*dbg*/ignoreUnused(index);
}

const String MainProcessor::getProgramName (int index)
{
    /*dbg*/ignoreUnused(index);
    /*dbg*/return String();
}

void MainProcessor::changeProgramName (int index, const String& newName)
{
    /*dbg*/ignoreUnused(index);
    /*dbg*/ignoreUnused(newName);
}

void MainProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    /*dbg*/ignoreUnused(destData);
}

void MainProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /*dbg*/ignoreUnused(data);
    /*dbg*/ignoreUnused(sizeInBytes);
}
