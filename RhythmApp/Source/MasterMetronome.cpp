/*
  ==============================================================================

    MasterMetronome.cpp
    Created: 14 Feb 2021 12:08:05am
    Author:  l33t h4x0r

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MasterMetronome.h"

//==============================================================================

MasterMetronome::MasterMetronome()
{
    //std::unique_ptr<Metronome> m = std::make_unique<Metronome> (new Metronome);
    //metronomes.emplace_back (std::make_unique<Metronome> (new Metronome));
    
    //Metronome m;
    //metronomes.push_back (m);
    metronomes.emplace_back (Metronome());
}

MasterMetronome::~MasterMetronome()
{
}

//==============================================================================

void MasterMetronome::paint (juce::Graphics& g)
{
    
}

void MasterMetronome::resized()
{

}

//==============================================================================

void MasterMetronome::setBufferSize (int newBufferSize)
{
    buffer.setSize (2, newBufferSize);
    
    for (auto& m : metronomes)
        m.setBufferSize (newBufferSize);
    
}

bool MasterMetronome::shouldClearBuffer()
{
    float scaledSubdivision         = 4.f * 60.f;
    float subdivisionInHertz        = masterTempo / scaledSubdivision;
    float subdivisionInSeconds      = 1.f / subdivisionInHertz;
    juce::int64 subdivisionInMillis = subdivisionInSeconds * 1000.f;
    
    timeSinceLastTick = juce::Time::currentTimeMillis();
    juce::int64 dt    = timeSinceLastTick - timeOfLastTick;
    
    if (dt > subdivisionInMillis)
    {
        timeOfLastTick = timeSinceLastTick;
        return true;
    }
    
    return false;
}

juce::AudioBuffer<float>& MasterMetronome::getBuffer()
{
    if (shouldClearBuffer())
        buffer.clear();
    
    else
    {
        
        int numChannels = buffer.getNumChannels();
        int numSamples  = buffer.getNumSamples();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);
            for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
            {
                float& sample = channelData[sampleIndex];
                for (auto& m : metronomes)
                {
                    juce::AudioBuffer<float>& mBuffer = m.getBuffer();
                    sample += mBuffer.getSample (0, sampleIndex);
                }
            } // end of sample loop
        } // end of channel loop
        
    } // end of else
    
    return buffer;
}

//==============================================================================
