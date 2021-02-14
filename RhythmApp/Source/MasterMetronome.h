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
    bool shouldClearBuffer();
    juce::AudioBuffer<float>& getBuffer();

    //==============================================================================

private:

//    using MetronomeVector = std::vector<std::unique_ptr<Metronome>>;
//    MetronomeVector metronomes;
    
    std::vector<Metronome> metronomes;
    
    //==============================================================================
    
    float masterTempo {60.f};
    
    //==============================================================================
    
    juce::int64 timeOfLastTick    {juce::Time::currentTimeMillis()};
    juce::int64 timeSinceLastTick {juce::Time::currentTimeMillis()};
    
    //==============================================================================
    
    juce::AudioBuffer<float> buffer;
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterMetronome)
};
