/*
  ==============================================================================

    MetronomeIncrementButton.h
    Created: 17 Feb 2021 6:17:45pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MetronomeButton : public juce::TextButton
{
public:
    
    //==============================================================================
    
    enum class Type
    {
        Increment,
        Decrement
    };
    
    //==============================================================================
    
    MetronomeButton (juce::Value& tempo, Type t) : masterTempo (tempo), type (t)
    {
        setClickingTogglesState (false);
                
        if (type == Type::Increment)
        {
            bgColour = juce::Colours::blue;
            setButtonText ("+");
            onClick = [this] ()
            {
                float newTempo = (float)masterTempo.getValue() + 1.f;
                masterTempo.setValue (newTempo);
            };
        }
            
        else if (type == Type::Decrement)
        {
            bgColour = juce::Colours::red;
            setButtonText ("-");
            onClick = [this] ()
            {
                float newTempo = (float)masterTempo.getValue() - 1.f;
                masterTempo.setValue (newTempo);
            };
        }
        
    }
    
    ~MetronomeButton() = default;
    
    //==============================================================================
    
    void paintButton (juce::Graphics& g, bool buttonIsHighlighted, bool buttonIsPressed) override
    {
        auto  r             = getLocalBounds().toFloat();
        float cornerSize    = 10.f;
        float lineThickness = 5.f;

        if      (buttonIsPressed)     g.setColour (bgColour.darker());
        else if (buttonIsHighlighted) g.setColour (bgColour.brighter());
        else                          g.setColour (bgColour);
        
        g.fillRoundedRectangle (r, cornerSize);
        
        g.setColour            (bgColour.darker().darker());
        g.drawRoundedRectangle (r, cornerSize, lineThickness);
        
        g.setColour (juce::Colours::white);
        g.setFont   (juce::Font{22.f});
        g.drawText  (this->getButtonText(), getLocalBounds(), juce::Justification::centred);
    }
    
    void resized() override
    {
    }
    
    //==============================================================================
  
private:
    
    juce::Value& masterTempo;
    Type type;
    juce::Colour bgColour;
};
