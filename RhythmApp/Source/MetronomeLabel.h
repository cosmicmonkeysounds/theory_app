/*
  ==============================================================================

    MetronomeTempoLabel.h
    Created: 17 Feb 2021 6:18:16pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MetronomeLabel : public juce::Label, public juce::Timer
{
public:
    
    //==============================================================================
    
    MetronomeLabel (juce::Value& tempo) : masterTempo (tempo)
    {
        startTimerHz (10);
        setEditable  (false);
        
        onTextChange = [this]
        {
            float tempoText = (float)getTextValue().getValue();
            float tempo     = std::ceil (tempoText * 10.f) / 10.f;
            masterTempo.setValue (tempo);
        };
    }
    
    ~MetronomeLabel()
    {
        stopTimer();
    }
    
    //==============================================================================
    
    void paint (juce::Graphics& g) override
    {
        auto area        = getLocalBounds();
        float cornerSize = 10.f;
        
        g.setColour            (juce::Colours::darkgrey.darker());
        g.fillRoundedRectangle (area.toFloat(), cornerSize);
        
        g.setColour (juce::Colours::white);
        g.setFont   (juce::Font {18.f});
        
        juce::String label {""};
        
        if (isBeingEdited())
        {
            if (shouldBlinkText)
            {
                label = currTextEditor->getText();
                label += "|";
            }
        }
            
        else
        {
            label = masterTempo.toString();
        }
            
        g.drawText (label, area, juce::Justification::centred);
    }
    
    void resized() override
    {
        
    }
    
    //==============================================================================
    
    void mouseDown (const juce::MouseEvent& e) override
    {
        showEditor();
        currTextEditor = getCurrentTextEditor();
        currTextEditor->clear();
    }
    
    //==============================================================================

    void timerCallback() override
    {
        juce::int64 currTime  = juce::Time::currentTimeMillis();
        juce::int64 dt        = currTime - lastBlinkTime;
        juce::int64 blinkTime = shouldBlinkText ? 300 : 150;
        
        if (dt > blinkTime)
        {
            shouldBlinkText = !shouldBlinkText;
            lastBlinkTime   = currTime;
        }
            
        repaint();
    }
    
    //==============================================================================
    
private:
    juce::Value&      masterTempo;
    juce::TextEditor* currTextEditor  {nullptr};
    bool              shouldBlinkText {false};
    juce::int64       lastBlinkTime   {juce::Time::currentTimeMillis()};
};
