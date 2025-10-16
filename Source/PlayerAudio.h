#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    bool loadFile(const juce::File& audioFile);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
