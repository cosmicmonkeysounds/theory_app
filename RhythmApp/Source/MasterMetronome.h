/*
  ==============================================================================

    MasterMetronome.h
    Created: 14 Feb 2021 12:08:05am
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Metronome.h"

class MasterMetronome  : public juce::Component
{
public:
    MasterMetronome();
    ~MasterMetronome() override;
    
    //==============================================================================

    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    
    void setBufferSize (int);
    juce::AudioBuffer<float>& getBuffer();

    //==============================================================================

private:

    std::vector<std::unique_ptr<Metronome>> metronomes;
    juce::AudioBuffer<float> buffer;
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterMetronome)
};
