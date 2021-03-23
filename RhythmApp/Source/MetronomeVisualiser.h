/*
  ==============================================================================

    MetronomeVisualiser.h
    Created: 22 Feb 2021 10:19:49pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MetronomeVisualiser : public juce::Component
{
public:
    
    //==============================================================================
    
    MetronomeVisualiser (int subs)
    {
        numberOfSubdivisions = subs;
        
        for (int block = 0; block < numberOfSubdivisions; ++block)
            blocks.emplace_back (juce::Rectangle<int>());
        
        juce::ColourGradient gradient = juce::ColourGradient::horizontal (juce::Colours::pink,
                                                                          0.f,
                                                                          juce::Colours::blue,
                                                                          20.f);
        
        colour = gradient.getColourAtPosition (numberOfSubdivisions);
    }
    
    //==============================================================================
    
    void paint (juce::Graphics& g) override
    {
        g.setColour (colour);
        for (auto block : blocks)
            g.fillRect (block);
    }
    
    void resized() override
    {
        
        auto r            = getLocalBounds();
        int  blockWidth   = r.getWidth() / numberOfSubdivisions;
        int  blockPadding = 2 * padding * numberOfSubdivisions;
        
        auto b = r.reduced    (blockPadding, padding)
                  .withWidth  (blockWidth)
                  .translated (-blockPadding, 0);
        
        for (auto& block : blocks)
        {
            block = b;
            b.translate (blockWidth + padding, 0);
        }
    }
    
    //==============================================================================
    
private:
    
    using BlockList = std::vector<juce::Rectangle<int>>;
    
    int          numberOfSubdivisions;
    int          padding {5};
    juce::Colour colour;
    BlockList    blocks;
};
