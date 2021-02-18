/*
  ==============================================================================

    MetronomeIncrementButton.h
    Created: 17 Feb 2021 6:17:45pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct MetronomeIncrementButton : juce::TextButton
{
        //==============================================================================
    
    MetronomeIncrementButton (bool inc) : shouldIncrement (inc)
    {
        this->setClickingTogglesState (false);
        
        if (shouldIncrement)
        {
            incrementFunc = [] (float& t) { t += 1.f; };
            this->setButtonText ("+");
            colour = juce::Colours::blue;
        }
            
        else
        {
            incrementFunc = [] (float& t) { t -= 1.f; };
            this->setButtonText ("-");
            colour = juce::Colours::red;
        }
    }
    
    ~MetronomeIncrementButton() = default;
    
    //==============================================================================
    
    void paintButton (juce::Graphics& g, bool buttonIsHighlighted, bool buttonIsPressed) override
    {
        float cornerSize         = 10.f;
        float lineThickness      = 5.f;
        juce::Rectangle<float> r = getLocalBounds().toFloat();
        
        if      (buttonIsPressed)     g.setColour (colour.darker());
        else if (buttonIsHighlighted) g.setColour (colour.brighter());
        else                          g.setColour (colour);
        
        g.fillRoundedRectangle (r, cornerSize);
        
        g.setColour (colour.darker().darker());
        g.drawRoundedRectangle (r, cornerSize, lineThickness);
        
        g.setColour (juce::Colours::white);
        g.setFont (juce::Font{22.f});
        g.drawText (this->getButtonText(), getLocalBounds(), juce::Justification::centred);
    }
    
    void resized() override
    {
    }
  
private:
    
    bool shouldIncrement;
    std::function<void(float&)> incrementFunc;
    juce::Colour colour;
};
