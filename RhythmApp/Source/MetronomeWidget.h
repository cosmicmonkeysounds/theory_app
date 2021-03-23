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

class MetronomeWidget  : public juce::Component,
                         public juce::Value::Listener,
                         public juce::Thread
{
public:
    
    //==============================================================================
    
    MetronomeWidget (juce::ValueTree& p) : Thread ("Metronome Thread"), parameters (p)
    {
        masterTempo.addListener      (this);
        metronomePlaying.addListener (this);
        
        masterTempo.setValue (120.f);
                
        addAndMakeVisible (incTempoBtn);
        addAndMakeVisible (decTempoBtn);
        addAndMakeVisible (tempoLabel);
        addAndMakeVisible (playButton);
        
        addAndMakeVisible (visualiser);
        
        setPriority (realtimeAudioPriority);
        startThread ();
        
        audioBuffer.clear();
    }
    
    ~MetronomeWidget() override
    {
        stopThread (100);
        masterTempo.removeListener (this);
    }
    
    //==============================================================================

    void paint (juce::Graphics& g) override
    {
        auto  area       = getLocalBounds().toFloat();
        float cornerSize = 10.f;
        
        g.setColour            (juce::Colours::white);
        g.drawRoundedRectangle (area, cornerSize, 2.f);
    }
    
    void resized() override
    {
        auto r      = getLocalBounds().reduced(5);
        int  width  = r.getWidth();
        int  height = r.getHeight();
        
        visualiser.setBounds  (r.removeFromBottom (height*0.2));
        
        float buttonWidth = width * 0.3;
        decTempoBtn.setBounds (r.removeFromLeft  (buttonWidth));
        incTempoBtn.setBounds (r.removeFromRight (buttonWidth));
    
        float tempoLabelHeight = height * 0.3;
        tempoLabel.setBounds  (r.removeFromTop (tempoLabelHeight));
        playButton.setBounds  (r.reduced (5, 5));
    }
    
    void run() override
    {
        while (true)
        {
            
            yield();
            if (threadShouldExit())
                return;
            
            if (metronomePlaying.getValue())
            {
                currentTime = juce::Time::currentTimeMillis();
                for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
                {
                    auto& m = kvPair->second;
                    juce::int64 dt = currentTime - m.timeOfLastTick;
                    if (dt > m.getSubdivisionInMillis())
                    {
                        m.trigger();
                        m.timeOfLastTick = currentTime;
                    }
                }
            }
            
        }
    }
    
    void valueChanged (juce::Value& value) override
    {
        if (value.refersToSameSourceAs (masterTempo))      setTempo (masterTempo.getValue());
        if (value.refersToSameSourceAs (metronomePlaying)) resetMetronomes();
    }
    
    //==============================================================================
    
    void setBufferSettings (int newBufferSize, double sampleRate)
    {
        audioBuffer.setSize (2, newBufferSize);
        
        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m = kvPair->second;
            m.sampler.setCurrentPlaybackSampleRate (sampleRate);
            m.loadSample (true);
        }
        
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
            m.timeOfLastTick = 0;
        }
    }
    
    //==============================================================================
        
    void addNewMetronome (int subdivision)
    {
        Metronome m  {subdivision, masterTempo.getValue()};
        metronomes[subdivision] = m;
    }
    
    //==============================================================================
    
    juce::AudioBuffer<float>& getBuffer()
    {
        //DBG ("hello");
        for (auto kvPair = metronomes.begin(); kvPair != metronomes.end(); ++kvPair)
        {
            auto& m = kvPair->second;
            m.sampler.renderNextBlock (audioBuffer, midiBuffer, 0, audioBuffer.getNumSamples());
        }
        
        return audioBuffer;
    }
    
private:
    
    //==============================================================================

    std::map<float, Metronome> metronomes;
    
    juce::AudioBuffer<float> audioBuffer;
    juce::MidiBuffer midiBuffer;
    
    juce::int64 currentTime = juce::Time::currentTimeMillis();
    
    //==============================================================================
    
    juce::ValueTree& parameters;
    juce::Value      masterTempo      {parameters.getPropertyAsValue ("masterTempo",      nullptr)};
    juce::Value      metronomePlaying {parameters.getPropertyAsValue ("metronomePlaying", nullptr)};
    
    //==============================================================================
    
    MetronomeLabel  tempoLabel  {masterTempo};
    MetronomeButton incTempoBtn {masterTempo, MetronomeButton::Type::Increment};
    MetronomeButton decTempoBtn {masterTempo, MetronomeButton::Type::Decrement};
    PlayButton      playButton  {metronomePlaying};
    
    MetronomeVisualiser visualiser {3};
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeWidget)
};
