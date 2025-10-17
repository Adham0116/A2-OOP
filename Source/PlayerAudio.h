#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    // These are the core audio processing functions
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    // These are the playback control functions
    bool loadFile(const juce::File& audioFile);
    void play();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    void restart();
    void setLooping(bool shouldLoop);
    double getPosition() const;
    double getLength() const;

private:
    bool isLooping = false;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};

