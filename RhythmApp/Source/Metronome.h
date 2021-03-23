/*
  ==============================================================================

    Metronome.h
    Created: 13 Feb 2021 10:22:10pm
    Author:  l33t h4x0r

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Metronome
{
public:
    
    //==============================================================================

    Metronome ()
    {
        //formatManager.clearFormats();
        formatManager.registerBasicFormats();
        
//        for (auto it = formatManager.begin(); it != formatManager.end(); ++it)
//            DBG (it->getFormatName());
            
        
        //DBG (formatManager.getNumKnownFormats());
        
        for (int voice = 0; voice < numVoices; ++voice)
        {
            sampler.addVoice (new juce::SamplerVoice());
        }
        
    }
    
    Metronome (int s, float t)
    {
        setTempo       (t);
        setSubdivision (s);
        Metronome();
    }
    
    ~Metronome()
    {
        formatReader = nullptr;
    }
    
    Metronome (const Metronome& other)
    {
        setTempo       (other.tempo);
        setSubdivision (other.subdivision);
        
        sampler.clearVoices();
        
        for (int voice = 0; voice < other.sampler.getNumVoices(); ++voice)
        {
            sampler.addVoice (other.sampler.getVoice (voice));
        }
    
        formatManager.clearFormats();
        formatManager.registerBasicFormats();

        if (other.formatReader != nullptr)
        {
            formatReader = other.formatReader;
        }
    }
    
    Metronome& operator= (const Metronome& other)
    {
        setTempo       (other.tempo);
        setSubdivision (other.subdivision);
        
        sampler.clearVoices();
        
        for (int voice = 0; voice < other.sampler.getNumVoices(); ++voice)
        {
            sampler.addVoice (other.sampler.getVoice (voice));
        }
    
        formatManager.clearFormats();
        formatManager.registerBasicFormats();

        if (other.formatReader != nullptr)
        {
            formatReader = other.formatReader;
        }

        return *this;
    }

    //==============================================================================
    
    int   getSubdivision         () { return subdivision; }
    float getSubdivisionInMillis () { return subdivisionInMillis; }
    
    void setTempo (float t)
    {
        tempo = t;                          // 240 = 4 * 60 = four quarters @ 60bpm,
        wholeNoteDuration = tempo / 240.f;  // aka: duration of 1 whole note @ 1 beat per second
    }
    
    void setSubdivision (int s)
    {
        subdivision = s;
        float subdivisionInHertz   = wholeNoteDuration * (float)subdivision;
        float subdivisionInSeconds = 1.f / subdivisionInHertz;
              subdivisionInMillis  = subdivisionInSeconds * 1000.f;
    }
    
    //==============================================================================
    
    void trigger()
    {
        DBG ("\nblip");
//        sampler.noteOn (1, 60, 127);
    }
    
    void loadSample (bool defaultSound)
    {
        if (defaultSound)
        {
            formatReader = formatManager.createReaderFor (juce::File {"../../../../Sounds/Click1.wav"});
        }
        
        else
        {
            juce::FileChooser chooser {"Please pick a sample to use"};
            if (chooser.browseForFileToOpen())
                formatReader = formatManager.createReaderFor (chooser.getResult());
        }
        
        juce::BigInteger range;
        range.setRange (0, 128, true);
        
        const int    c3      = 60;
        const double attack  = 0.1;
        const double release = 1.0;
        const double maxTime = release;
        
        sampler.addSound (new juce::SamplerSound ("Sample", *formatReader, range,
                                                  c3, attack, release, maxTime));
    }
    
    //==============================================================================
    
    juce::int64 timeOfLastTick  {juce::Time::currentTimeMillis()};
    
    juce::Synthesiser sampler;
    const int numVoices = 3;
    
    //==============================================================================
    
private:
    
    //==============================================================================
    
    int   subdivision  {4};
    float tempo        {120.f};
    float subdivisionInMillis;
    float wholeNoteDuration;
    
    juce::File click;
    
    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* formatReader {nullptr};
    
    //==============================================================================
    
};
