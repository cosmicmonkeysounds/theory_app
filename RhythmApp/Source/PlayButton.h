/*
  ==============================================================================

    PlayButton.h
    Created: 21 Feb 2021 11:41:28am
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once

class PlayButton : public juce::Component
{
public:
    
    //==============================================================================
    
    PlayButton (juce::Value& play) : shouldPlay (play)
    {
        shouldPlay.setValue (false);
    }
    
    //==============================================================================
    
    void paint (juce::Graphics& g) override
    {
        auto r          = getLocalBounds();
        int  width      = r.getWidth();
        int  height     = r.getHeight();
        auto cornerSize = 10.f;
        
        // BACKGROUND
        g.setColour            (colour);
        g.fillRoundedRectangle (r.toFloat(), cornerSize);
        
        // DRAW ICON
        int   iconWidthHeight = juce::jmin (width, height) * 0.5;
        auto  iconBounds      = r.withSizeKeepingCentre (iconWidthHeight, iconWidthHeight).toFloat();
        float iconWidth       = iconBounds.getWidth();
        float iconHeight      = iconBounds.getHeight();
        
        auto topLeft = iconBounds.getTopLeft();
        juce::Path path;
        path.startNewSubPath (topLeft);
        
        bool playing = shouldPlay.getValue();
                
        if (playing)
        {
            path.addRoundedRectangle (iconBounds, 2.f);
        }
        
        else
        {
            auto bottomLeft  = topLeft + juce::Point<float> {0, iconHeight};
            auto centreRight = topLeft + juce::Point<float> {iconWidth, iconHeight/2.f};
            
            path.lineTo (bottomLeft);
            path.lineTo (centreRight);
            path.closeSubPath();
        }
                
        g.setColour (colour.darker());
        g.fillPath  (path);
    }
    
    void resized() override
    {

    }
    
    //==============================================================================

    void mouseDown (const juce::MouseEvent& e) override
    {
        bool playing = shouldPlay.getValue();
        
        if (playing) colour = juce::Colours::green;
        else         colour = juce::Colours::red;
        
        repaint();
        shouldPlay.setValue (!playing);
    }
    
    void mouseEnter (const juce::MouseEvent& e) override { colour = colour.brighter(); }
    void mouseExit  (const juce::MouseEvent& e) override { colour = colour.darker(); }
    
    //==============================================================================
    
private:
    juce::Value& shouldPlay;
    juce::Colour colour {juce::Colours::green};
};
