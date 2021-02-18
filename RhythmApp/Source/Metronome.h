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
    Metronome (float s) : subdivision (s)
    {
        Metronome();
    }
    
    ~Metronome()                            = default;
    
    Metronome (const Metronome&)            = default;
    Metronome& operator= (const Metronome&) = default;
    
    Metronome (Metronome&&)                 = default;
    Metronome& operator= (Metronome&&)      = default;
    
    //==============================================================================
    
    void setBufferSize (int newNumSamples)
    {
        buffer.setSize (1, newNumSamples);
        
    }
    
    juce::AudioBuffer<float>& getBuffer()
    {
        // just playing white noise, for now
        if (shouldPlayTick())
        {
            float* channelData = buffer.getWritePointer(0);
            for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); ++sampleIndex )
            {
                float& sample = channelData[sampleIndex];
                sample = random.nextFloat();
            }
        }
        
        else
        {
            buffer.clear();
        }
        
        return buffer;
    }

    //==============================================================================
    
    void setTempo        (float t) { tempo = t; }
    
    void  setSubdivision (float s) { subdivision = s; }
    float getSubdivision ()        { return subdivision; }

    //==============================================================================

private:
    
    float tempo       {120.f};
    float subdivision {4.f};

    //==============================================================================
    
    juce::int64 timeOfLastTick    {juce::Time::currentTimeMillis()};
    juce::int64 timeSinceLastTick {juce::Time::currentTimeMillis()};

    //==============================================================================
    
    juce::AudioBuffer<float> buffer;
    juce::Random random;

    //==============================================================================
    
    bool shouldPlayTick()
    {
        timeSinceLastTick = juce::Time::currentTimeMillis();
        juce::int64 dt    = timeSinceLastTick - timeOfLastTick;
        
        if (dt > getSubdivisionInMillis())
        {
            timeOfLastTick = timeSinceLastTick;
            return true;
        }
        
        return false;
    }
    
    // 240 = 4 * 60: four quarters @ 60bpm = 1 whole note per second
    float getWholeNotePerSecond() { return tempo / 240.f; }
    
    float getSubdivisionInMillis()
    {
        float subdivisionInHertz   = getWholeNotePerSecond() * subdivision;
        float subdivisionInSeconds = 1.f / subdivisionInHertz;
        
        return subdivisionInSeconds * 1000.f;
    }

    //==============================================================================
    
};
