#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI 
  : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // Listener Callbacks
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    // These are the declarations for the audio functions that MainComponent will call.
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;

    // GUI Elements
    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Slider gainSlider;
    juce::TextButton restartButton;
    juce::ToggleButton loopButton;
    juce::TextButton muteButton;
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};

