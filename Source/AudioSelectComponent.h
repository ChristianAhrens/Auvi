/*
  ==============================================================================

    AudioSelectComponent.h
    Created: 11 Nov 2019 8:43:14pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Auvi
{

//==============================================================================
/*
*/
class AudioSelectComponent    : public AudioDeviceSelectorComponent
{
public:
    AudioSelectComponent(	AudioDeviceManager *deviceManager,
							int minAudioInputChannels,
							int maxAudioInputChannels,
							int minAudioOutputChannels,
							int maxAudioOutputChannels,
							bool showMidiInputOptions,
							bool showMidiOutputSelector,
							bool showChannelsAsStereoPairs,
							bool hideAdvancedOptionsWithButton);
    ~AudioSelectComponent();

    void paint (Graphics&) override;
    void resized() override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSelectComponent)
};

}