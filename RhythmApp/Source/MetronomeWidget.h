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
#include "MetronomeButton.h"
#include "PlayButton.h"
#include "MetronomeLabel.h"
#include "MetronomeVisualiser.h"

class MetronomeWidget  : public juce::Component, juce::Value::Listener
{
public:
    
    //==============================================================================
    
    MetronomeWidget (juce::ValueTree& p) : parameters (p)
    {
        masterTempo.setValue         (120.f);
        masterTempo.addListener      (this);
        metronomePlaying.addListener (this);
        
        addNewMetronome (4.f);
        addNewMetronome (3.f);
                
        addAndMakeVisible (incTempoBtn);
        addAndMakeVisible (decTempoBtn);
        addAndMakeVisible (tempoLabel);
        addAndMakeVisible (playButton);
        
        addAndMakeVisible (visualiser);
    }
    
    ~MetronomeWidget() override
    {
        masterTempo.removeListener (this);
    }
    
    //==============================================================================

    void paint (juce::Graphics& g) override
    {
        auto area        = getLocalBounds().toFloat();
        float cornerSize = 10.f;
        
        g.setColour            (juce::Colours::white);
        g.drawRoundedRectangle (area, cornerSize, 2.f);
    }
    
    void resized() override
    {
        auto r      = getLocalBounds().reduced(5);
        int  width  = r.getWidth();
        int  height = r.getHeight();
        
        visualiser.setBounds (r.removeFromBottom (height*0.2));
        
        float buttonWidth = width * 0.3;
        decTempoBtn.setBounds (r.removeFromLeft  (buttonWidth));
        incTempoBtn.setBounds (r.removeFromRight (buttonWidth));
    
        float tempoLabelHeight = height * 0.3;
        tempoLabel.setBounds  (r.removeFromTop (tempoLabelHeight));
        playButton.setBounds  (r.reduced (5, 5));
    }

    //==============================================================================
    
    void valueChanged (juce::Value& value) override
    {
        if (value.refersToSameSourceAs (masterTempo))      setTempo (masterTempo.getValue());
        if (value.refersToSameSourceAs (metronomePlaying)) resetMetronomes();
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
        
        if (metronomePlaying.getValue())
        {
            for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
            {
                auto& m       = kvPair->second;
                auto& mBuffer = m.getBuffer();

                for (int channel = 0; channel < numChannels; ++channel)
                    buffer.addFrom (channel, 0, mBuffer, 0, 0, numSamples);
            }
        }
        
        return buffer;
    }

    //==============================================================================
    
    void setTempo (float newTempo)
    {
        parameters.setProperty ("masterTempo", newTempo, nullptr);
        
        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m = kvPair->second;
            m.setTempo (newTempo);
        }
    }
    
    void resetMetronomes()
    {
        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m = kvPair->second;
            m.resetTiming();
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

    std::map<float, Metronome> metronomes;
    juce::AudioBuffer<float> buffer;
    
    //==============================================================================
    
    juce::ValueTree& parameters;
    juce::Value masterTempo      {parameters.getPropertyAsValue ("masterTempo", nullptr)};
    juce::Value metronomePlaying {parameters.getPropertyAsValue ("metronomePlaying", nullptr)};
    
    //==============================================================================
    
    MetronomeLabel  tempoLabel  {masterTempo};
    MetronomeButton incTempoBtn {masterTempo, MetronomeButton::Type::Increment};
    MetronomeButton decTempoBtn {masterTempo, MetronomeButton::Type::Decrement};
    PlayButton      playButton  {metronomePlaying};
    
    MetronomeVisualiser visualiser {3};
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeWidget)
};
