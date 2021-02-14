/*
  ==============================================================================

    Metronome.cpp
    Created: 13 Feb 2021 10:22:10pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Metronome.h"

//==============================================================================
Metronome::Metronome()
{
    buffer.clear();

}

Metronome::~Metronome()
{
}

void Metronome::paint (juce::Graphics& g)
{
    
}

void Metronome::resized()
{

}

juce::AudioBuffer<float>& Metronome::getBuffer()
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

void Metronome::setBufferSize (int numSamples)
{
    buffer.setSize (1, numSamples);
}

bool Metronome::shouldPlayTick()
{
    timeSinceLastTick = juce::Time::currentTimeMillis();
    juce::int64 dt = timeSinceLastTick - timeOfLastTick;
    
    if (dt > getNoteDurationInMillis())
    {
        timeOfLastTick = timeSinceLastTick;
        return true;
    }
    
    return false;
}

float Metronome::getWholeNotePerSecond()
{
    float scaledSubdivision = subdivisionOfTempo * 60.f;
    return tempo / scaledSubdivision;
}

float Metronome::getNoteDurationInMillis()
{
    float subdivisionInHertz = getWholeNotePerSecond() * notesPerWholeNote;
    float subdivisionInSeconds = 1.f / subdivisionInHertz;
    
    return subdivisionInSeconds * 1000.f;
}