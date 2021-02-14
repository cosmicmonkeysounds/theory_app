/*
  ==============================================================================

    Metronome.h
    Created: 13 Feb 2021 10:22:10pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Metronome
{
public:
    //==============================================================================

    Metronome();
    ~Metronome();

    //==============================================================================
    
    void setBufferSize (int);
    juce::AudioBuffer<float>& getBuffer();

    //==============================================================================
    
    void setTempo(float);
    void setSubdivision(float);
    void setNotesPerWholeNote(float);

    //==============================================================================

private:
    
    float tempo              {60.f};
    float subdivisionOfTempo {4.f};
    float notesPerWholeNote  {8.f};

    //==============================================================================
    
    juce::int64 timeOfLastTick    {juce::Time::currentTimeMillis()};
    juce::int64 timeSinceLastTick {juce::Time::currentTimeMillis()};

    //==============================================================================
    
    juce::AudioBuffer<float> buffer;
    juce::Random random;

    //==============================================================================
    
    bool shouldPlayTick();
    float getWholeNotePerSecond();
    float getNoteDurationInMillis();

    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Metronome)
};
