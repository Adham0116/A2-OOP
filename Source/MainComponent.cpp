#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(600, 400);

    // Request audio permissions
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels(0, 2); // Grant audio output channels
    }

    addAndMakeVisible(playerGUI);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
// These are the missing function definitions that fix the linker errors.
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerGUI.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerGUI.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    playerGUI.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    playerGUI.setBounds(getLocalBounds());
}
