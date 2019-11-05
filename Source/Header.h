/*
  ==============================================================================

    MainHeader.h
    Created: 1 Nov 2019 11:34:46am
    Author:  Christian Ahrens

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Header    : public Component
{
public:
    class Listener
    {
    public:
        virtual ~Listener(){};
        //virtual void doSomething() = 0;
    };
    
public:
    Header();
    ~Header();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};
