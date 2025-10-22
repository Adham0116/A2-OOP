#include "PlayerAudio.h"

PlayerAudio::PlayerAudio(){
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio(){
    transportSource.pause();
    transportSource.setSource(nullptr);
    readerSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill){
    if (readerSource != nullptr && transportSource.hasStreamFinished()){
        if (isLooping){
            transportSource.setPosition(0.0);
            transportSource.start();
        }
    }
    if (readerSource == nullptr){
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources(){
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& audioFile){
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    juce::AudioFormatReader* reader = formatManager.createReaderFor(audioFile);

    if (reader == nullptr)
        return false;

    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

    readerSource.reset(newSource.release());

    return true;
}
bool PlayerAudio::isPlaying() const {
    return transportSource.isPlaying();
}

void PlayerAudio::play(){
    transportSource.start();
}

void PlayerAudio::pause(){
    transportSource.pause();
}

void PlayerAudio::restart(){
    transportSource.setPosition(0.0);
}

void PlayerAudio::setGain(float gain){
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos){
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::goToStart(){
    transportSource.setPosition(0.0);
}

void PlayerAudio::goToEnd(){
    transportSource.setPosition(getLength());
}

void PlayerAudio::setLooping(bool shouldLoop){
    isLooping = shouldLoop;
}

void PlayerAudio::mute(){
    if (isMuted){
        transportSource.setGain(currentGain);
    }
    else{
        currentGain = transportSource.getGain();
        transportSource.setGain(0.0f);
	}
	isMuted = !isMuted;
}
