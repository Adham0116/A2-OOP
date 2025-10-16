#include "PlayerGUI.h"

PlayerGUI::PlayerGUI() 
    : playButton("Play"),
    stopButton("Stop")
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(gainSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    gainSlider.addListener(this);

    gainSlider.setRange(0.0, 1.0, 0.01);
    gainSlider.setValue(0.5);

    setSize(400, 200);
}

PlayerGUI::~PlayerGUI()
{
    playButton.removeListener(this);
    stopButton.removeListener(this);
    gainSlider.removeListener(this);
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
}

void PlayerGUI::resized()
{
    playButton.setBounds(10, 10, 80, 30);
    stopButton.setBounds(100, 10, 80, 30);
    gainSlider.setBounds(10, 50, 170, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton) playerAudio.start();
    else if (button == &stopButton) playerAudio.stop();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider) playerAudio.setGain(gainSlider.getValue());
}

