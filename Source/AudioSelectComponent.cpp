/*
  ==============================================================================

    AudioSelectComponent.cpp
    Created: 11 Nov 2019 8:43:14pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "AudioSelectComponent.h"

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
AudioSelectComponent::AudioSelectComponent(AudioDeviceManager& deviceManager, int minAudioInputChannels, int maxAudioInputChannels, int minAudioOutputChannels, 
	int maxAudioOutputChannels, bool showMidiInputOptions, bool showMidiOutputSelector, bool showChannelsAsStereoPairs, bool hideAdvancedOptionsWithButton)
	: AudioDeviceSelectorComponent(deviceManager, minAudioInputChannels, maxAudioInputChannels, minAudioOutputChannels, 
		maxAudioOutputChannels, showMidiInputOptions, showMidiOutputSelector, showChannelsAsStereoPairs, hideAdvancedOptionsWithButton)
{
}

AudioSelectComponent::~AudioSelectComponent()
{
}

void AudioSelectComponent::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).darker());

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	AudioDeviceSelectorComponent::paint(g);
}

void AudioSelectComponent::resized()
{
	AudioDeviceSelectorComponent::resized();
}
