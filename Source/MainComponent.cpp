#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(playerGUI);
    setSize(500, 300);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

void MainComponent::resized()
{
    playerGUI.setBounds(getLocalBounds());
}

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

