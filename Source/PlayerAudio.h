#pragma once
#include <JuceHeader.h>

class PlayerAudio{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& audioFile);
    void play();
    bool isPlaying() const;
    void pause();
    void setGain(float gain);
    void setPosition(double pos);
    void restart();
    void setLooping(bool shouldLoop);
    void goToStart();
    void goToEnd();
    double getPosition() const;
    double getLength() const;
    void mute();

    bool isMuted = false;


private:
    bool isLooping = false;
	float currentGain = 1.0f;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};

