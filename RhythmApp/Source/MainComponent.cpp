#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() : metronome (parameters)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (getParentWidth(), getParentHeight());

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    metronome.addNewMetronome (4);
    addAndMakeVisible (metronome);
    
    parameters.setProperty (juce::Identifier {"masterTempo"}, 120.f, nullptr);
    parameters.setProperty (juce::Identifier {"metronomePlaying"}, false, nullptr);
    
}

//==============================================================================

MainComponent::~MainComponent()
{
    metronome.signalThreadShouldExit();
    shutdownAudio();
}

//==============================================================================

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    metronome.setBufferSettings (samplesPerBlockExpected, sampleRate);
}

//==============================================================================

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    //bufferToFill.clearActiveBufferRegion();
    
    juce::AudioBuffer<float>& buffer          = *bufferToFill.buffer;
    juce::AudioBuffer<float>& metronomeBuffer = metronome.getBuffer();

    int numChannels = buffer.getNumChannels();
    int numSamples  = buffer.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        buffer.addFrom (channel, 0, metronomeBuffer, 0, 0, numSamples);
    }

} // end of processing

//==============================================================================

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

//==============================================================================

void MainComponent::resized()
{
    auto r = getLocalBounds();
    
    metronome.setBounds (r.withSizeKeepingCentre (r.getWidth()/3, r.getHeight()/4));
}

//==============================================================================
