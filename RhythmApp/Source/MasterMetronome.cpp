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
    Metronome m;
    //std::pair<float, Metronome*> mp (m.getSubdivision(), &m);
    metronomes[m.getSubdivision()] = m;
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

    for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
    {
        auto& m = kvPair->second;
        m.setBufferSize (newBufferSize);
    }
        
}

juce::AudioBuffer<float>& MasterMetronome::getBuffer()
{
    buffer.clear();

    int numChannels = buffer.getNumChannels();
    int numSamples  = buffer.getNumSamples();
    
    for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
    {
        auto& m       = kvPair->second;
        auto& mBuffer = m.getBuffer();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            buffer.addFrom (channel, 0, mBuffer, 0, 0, numSamples);
        }
    }
        
    return buffer;
}

//==============================================================================
