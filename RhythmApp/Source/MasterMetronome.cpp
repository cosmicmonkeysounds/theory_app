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
    std::unique_ptr<Metronome> m = std::make_unique<Metronome> (new Metronome);
    metronomes.push_back (m);
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
    {
        m.get()->setBufferSize (newBufferSize);
    }
        
}

juce::AudioBuffer<float>& MasterMetronome::getBuffer()
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
                juce::AudioBuffer<float>& mBuffer = m.get()->getBuffer();
                sample += mBuffer.getSample (0, sampleIndex);
            }
        } // end of sample loop

    } // end of channel loop
    
    return buffer;
}

//==============================================================================
