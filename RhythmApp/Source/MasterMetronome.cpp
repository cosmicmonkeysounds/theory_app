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
    addNewMetronome (4.f);
    
    addAndMakeVisible (tempoLabel);
    tempoLabel.setJustificationType (juce::Justification::centred);
    tempoLabel.setText (juce::String (masterTempo), juce::NotificationType::dontSendNotification);
    tempoLabel.setEditable (true);
    tempoLabel.onTextChange = [this] { setTempo ((float)tempoLabel.getTextValue().getValue()); };
    
    addAndMakeVisible (incTempoBtn);
    incTempoBtn.setClickingTogglesState (false);
    incTempoBtn.setButtonText ("+");
    
    addAndMakeVisible (decTempoBtn);
    decTempoBtn.setClickingTogglesState (false);
    decTempoBtn.setButtonText ("-");
    
    incTempoBtn.onClick = [this]
    {
        setTempo (masterTempo += 1.f);
        tempoLabel.setText (juce::String (masterTempo), juce::NotificationType::dontSendNotification);
    };
    
    decTempoBtn.onClick = [this]
    {
        setTempo (masterTempo -= 1.f);
        tempoLabel.setText (juce::String (masterTempo), juce::NotificationType::dontSendNotification);
    };
    
}

MasterMetronome::~MasterMetronome()
{
}

//==============================================================================

void MasterMetronome::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkblue);
}

void MasterMetronome::resized()
{
    auto r = getLocalBounds();
    auto width = r.getWidth();
    
    decTempoBtn.setBounds (r.removeFromLeft (width*0.3));
    tempoLabel.setBounds  (r.removeFromLeft (width*0.4));
    incTempoBtn.setBounds (r);
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

void MasterMetronome::setTempo (float newTempo)
{
    masterTempo = newTempo;
    
    for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
    {
        auto& m = kvPair->second;
        m.setTempo (masterTempo);
    }
}

//==============================================================================

void MasterMetronome::addNewMetronome (Metronome& newMetronome)
{
    metronomes[newMetronome.getSubdivision()] = newMetronome;
}

void MasterMetronome::addNewMetronome (float subdivision)
{
    Metronome m {subdivision};
    addNewMetronome (m);
}

//==============================================================================
