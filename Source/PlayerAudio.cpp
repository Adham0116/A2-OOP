#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    // Register the basic audio formats (like WAV, AIFF, etc.)
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    // Clean up resources
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare the transport source for playback
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource != nullptr && transportSource.hasStreamFinished())
    {
        // If the track is over AND our looping flag is true, we reset the
        // playback position to the very beginning (0.0 seconds).
        if (isLooping)
        {
            transportSource.setPosition(0.0);
        }
    }
    // If there's no audio file loaded, just clear the buffer to avoid noise.
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    // Otherwise, get the next block of audio from the transport source.
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    // Release any resources when playback stops.
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& audioFile)
{
    // Stop playback and release the current file before loading a new one.
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    // Create a reader for the new audio file.
    juce::AudioFormatReader* reader = formatManager.createReaderFor(audioFile);

    // Check if the file was successfully opened.
    if (reader == nullptr)
        return false;

    // Create a source that can stream from the reader.
    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

    // Set this new source as the input for our transport source.
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

    // Take ownership of the new source so it doesn't get deleted prematurely.
    readerSource.reset(newSource.release());

    return true;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::restart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}
void PlayerAudio::mute()
{
    if (isMuted)
    {
        transportSource.setGain(currentGain);
    }
    else
    {
        currentGain = transportSource.getGain();
        transportSource.setGain(0.0f);
	}
	isMuted = !isMuted;

}



