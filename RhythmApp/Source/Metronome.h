/*
  ==============================================================================

    Metronome.h
    Created: 13 Feb 2021 10:22:10pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Metronome
{
public:
    
    //==============================================================================

    Metronome () {}
    Metronome (int s) : subdivision (s)
    {
        Metronome();
    }
    
    ~Metronome()                            = default;
    
    Metronome            (const Metronome&) = default;
    Metronome& operator= (const Metronome&) = default;
    
    Metronome            (Metronome&&)      = default;
    Metronome& operator= (Metronome&&)      = default;

    //==============================================================================
    
    int  getSubdivision ()        { return subdivision; }
    void setSubdivision (float s) { subdivision = s; }
    void setTempo       (float t) { tempo = t; }

    //==============================================================================
    
    float getWholeNoteDuration()
    {
        // 240 = 4 * 60 = four quarters @ 60bpm,
        // aka: duration of 1 whole note @ 1 beat per second
        return tempo / 240.f;
    }
    
    float getSubdivisionInMillis()
    {
        float  subdivisionInHertz   = getWholeNoteDuration() * (float)subdivision;
        float  subdivisionInSeconds = 1.f / subdivisionInHertz;
        return subdivisionInSeconds * 1000.f;
    }
    
    //==============================================================================
    
    void trigger()
    {
        DBG ("\nblip");
    }
    
    //==============================================================================
    
    juce::int64 timeOfLastTick  {juce::Time::currentTimeMillis()};
    
    //==============================================================================
    
private:
    
    //==============================================================================
    
    int   subdivision  {4};
    float tempo        {120.f};
    
    //==============================================================================
    
};
