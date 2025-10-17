#pragma once

#include <JuceHeader.h>
#include "PlayerGUI.h"

//==============================================================================
// This component MUST inherit from AudioAppComponent to handle audio.
class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    // Core audio methods to connect to the hardware.
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PlayerGUI playerGUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};


