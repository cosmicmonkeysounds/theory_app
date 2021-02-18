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
#include "MetronomeIncrementButton.h"
#include "MetronomeTempoLabel.h"

class MetronomeWidget  : public juce::Component
{
public:
    MetronomeWidget()
    {
        addNewMetronome (4.f);
        
        addAndMakeVisible (tempoLabel);
        tempoLabel.setJustificationType (juce::Justification::centred);
        tempoLabel.setText (juce::String (masterTempo), juce::NotificationType::dontSendNotification);
        tempoLabel.setEditable (true);
        tempoLabel.onTextChange = [this] { setTempo ((float)tempoLabel.getTextValue().getValue()); };
        
        addAndMakeVisible (incTempoBtn);
        addAndMakeVisible (decTempoBtn);
        
    }
    
    ~MetronomeWidget() override {}
    
    //==============================================================================

    void paint (juce::Graphics& g) override
    {
        float cornerSize         = 10.f;
        juce::Rectangle<float> r = getLocalBounds().toFloat();
        
        g.setColour (juce::Colours::darkgrey.darker());
        g.fillRoundedRectangle (r, cornerSize);
    }
    
    void resized() override
    {
        auto r = getLocalBounds();
        auto width = r.getWidth();
        
        decTempoBtn .setBounds (r.removeFromLeft (width*0.3));
        tempoLabel  .setBounds (r.removeFromLeft (width*0.4));
        incTempoBtn .setBounds (r);
    }

    //==============================================================================
    
    void setBufferSize (int newBufferSize)
    {
        buffer.setSize (2, newBufferSize);

        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m = kvPair->second;
            m.setBufferSize (newBufferSize);
        }
            
    }
    
    juce::AudioBuffer<float>& getBuffer()
    {
        buffer.clear();

        int numChannels = buffer.getNumChannels();
        int numSamples  = buffer.getNumSamples();
        
        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m       = kvPair->second;
            auto& mBuffer = m.getBuffer();

            for (int channel = 0; channel < numChannels; ++channel)
                buffer.addFrom (channel, 0, mBuffer, 0, 0, numSamples);
        }
            
        return buffer;
    }

    //==============================================================================
    
    void setTempo (float newTempo)
    {
        masterTempo = newTempo;
        
        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m = kvPair->second;
            m.setTempo (masterTempo);
        }
    }
    
    //==============================================================================
    
    void addNewMetronome (Metronome& newMetronome)
    {
        metronomes[newMetronome.getSubdivision()] = newMetronome;
    }
    
    void addNewMetronome (float subdivision)
    {
        Metronome m {subdivision};
        addNewMetronome (m);
    }
    
    //==============================================================================
    
private:
    
    //==============================================================================
    
    using MetronomeMap = std::map<float, Metronome>;
    MetronomeMap metronomes;
    
    //==============================================================================
    
    juce::Label tempoLabel;
    
    MetronomeIncrementButton incTempoBtn {true}, decTempoBtn {false};
    
    //==============================================================================
    
    float masterTempo {120.f};
    
    //==============================================================================
    
    juce::AudioBuffer<float> buffer;
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeWidget)
};
