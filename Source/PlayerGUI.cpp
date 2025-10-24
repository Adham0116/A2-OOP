#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
  : loadButton("Load"),
    playButton("Play"),
    pauseButton("Pause"),
    restartButton("Restart"),
    muteButton("Mute"),
    goToStart("|<"),
    goToEnd(">|"){

    // Make buttons & slider visible
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(gainSlider);
	addAndMakeVisible(restartButton);
	addAndMakeVisible(loopButton);
	addAndMakeVisible(muteButton);
	addAndMakeVisible(goToStart);
	addAndMakeVisible(goToEnd);


    // Make this class listen for clicks & slider movements
    loadButton.addListener(this);
    playButton.addListener(this);
    pauseButton.addListener(this);
    gainSlider.addListener(this);
    loopButton.addListener(this);
    restartButton.addListener(this);
	muteButton.addListener(this);
	goToStart.addListener(this);
	goToEnd.addListener(this);

    // Configure the gain slider.
    gainSlider.setRange(0.0, 1.0, 0.01);
    gainSlider.setValue(0.5);
    // Give the button a label
    loopButton.setButtonText("Loop");
    // This makes the button stay "down" when clicked, visually showing its on/off state.
    loopButton.setClickingTogglesState(true);
    // Set the initial gain on the audio player.
    playerAudio.setGain((float)gainSlider.getValue());
}

// Destructors
PlayerGUI::~PlayerGUI(){
    loadButton.removeListener(this);
    playButton.removeListener(this);
    pauseButton.removeListener(this);
    gainSlider.removeListener(this);
    restartButton.removeListener(this);
    loopButton.removeListener(this);
	muteButton.removeListener(this);
    goToStart.removeListener(this);
	goToEnd.removeListener(this);
}

//==============================================================================

//Background Color
void PlayerGUI::paint(juce::Graphics& g){
    g.fillAll(juce::Colours::darkgrey);
}

// Layout Design
void PlayerGUI::resized(){
    const int padding = 10;
    const int buttonWidth = 100;
    const int buttonHeight = 100;
    int xPosition = padding;
    int yPosition = padding;

    //First Row
    loadButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;
    goToStart.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;
    playButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;
    pauseButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;
    goToEnd.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;

    // Second Row
    xPosition = padding;
    yPosition += buttonHeight + padding;

    restartButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;
    muteButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);
    xPosition += buttonWidth + padding;
    loopButton.setBounds(xPosition, yPosition, buttonWidth, buttonHeight);

    //Third Row
    yPosition += buttonHeight + padding;
    gainSlider.setBounds(padding, yPosition, getWidth() - (2 * padding), buttonHeight);

}

//==============================================================================

// Buttons Logic
void PlayerGUI::buttonClicked(juce::Button* button){
    if (button == &loadButton){
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.aiff");

        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser){
                juce::File result = chooser.getResult();
                if (result.exists()){
                    playerAudio.loadFile(result);
                }
            });
    }
	else if (button == &goToStart){
        playerAudio.goToStart();
	}
    else if (button == &playButton){
        playerAudio.play();
    }
    else if (button == &pauseButton){
        playerAudio.pause();
    }
    else if (button == &goToEnd){
        playerAudio.goToEnd();
	}

    else if (button == &restartButton) {
        playerAudio.restart();
    }
	else if (button == &muteButton){
        playerAudio.mute(); 
        if (playerAudio.isMuted)
            muteButton.setButtonText("Unmute");
        else
            muteButton.setButtonText("Mute");
	}
    else if (button == &loopButton){
        playerAudio.setLooping(loopButton.getToggleState());
    }
}

//==============================================================================

// Slider Function
void PlayerGUI::sliderValueChanged(juce::Slider* slider){
    if (slider == &gainSlider){
        playerAudio.setGain((float)gainSlider.getValue());
    }
}

//==============================================================================
// MainComponent connection to PlayerAudio
//==============================================================================

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill){
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources(){
    playerAudio.releaseResources();
}
