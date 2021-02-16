/*
  ==============================================================================

    MasterMetronome.h
    Created: 14 Feb 2021 12:08:05am
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <map>
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
    
    void setTempo (float);
    
    //==============================================================================
    
    void addNewMetronome (Metronome&);
    void addNewMetronome (float);
    
    //==============================================================================
    
private:
    
    //==============================================================================
    
    using MetronomeMap = std::map<float, Metronome>;
    MetronomeMap metronomes;
    
    //==============================================================================
    
    juce::Label tempoLabel;
    juce::TextButton incTempoBtn, decTempoBtn;
    
    //==============================================================================
    
    float masterTempo {120.f};
    
    //==============================================================================
    
    juce::AudioBuffer<float> buffer;
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterMetronome)
};
