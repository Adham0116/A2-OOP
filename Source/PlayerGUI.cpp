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
    addAndMakeVisible(playlist);


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
    playlist.setModel(this);

    // Configure the gain slider.
    gainSlider.setRange(0.0, 1.0, 0.01);
    gainSlider.setValue(0.5);
    // Give the button a label
    loopButton.setButtonText("Loop");
    // This makes the button stay "down" when clicked, visually showing its on/off state.
    loopButton.setClickingTogglesState(true);
    // Set the initial gain on the audio player.
    playerAudio.setGain((float)gainSlider.getValue());
	// Track title constructor
    addAndMakeVisible(trackTitle);
    trackTitle.setText("No file loaded", juce::dontSendNotification);
    trackTitle.setJustificationType(juce::Justification::centred);
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
    const int controlsWidth = getWidth() / 2;
    const int playlistWidth = getWidth() - controlsWidth - padding;

	// Playlist area
    playlist.setBounds(controlsWidth,padding,playlistWidth,getHeight() - (2 * padding));


	// Controls area
    const int componentWidth = controlsWidth - (2 * padding);
    int xPosition = padding;
    int yPosition = padding;

    // Row 1
    trackTitle.setBounds(xPosition, yPosition, componentWidth, 30);
    yPosition += 30 + padding;

    // Row 2
    gainSlider.setBounds(xPosition, yPosition, componentWidth, 30);
    yPosition += 30 + padding;

    // --- Button Grid ---
    const int buttonWidth = (componentWidth - (2 * padding)) / 3;
    const int buttonHeight = 40;
    int buttonX = xPosition;

        // Button Row 1: Go To Start, Play, Pause
    goToStart.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);
    buttonX += buttonWidth + padding;
    playButton.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);
    buttonX += buttonWidth + padding;
    pauseButton.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);

    yPosition += buttonHeight + padding; 
    buttonX = xPosition;                

        // Button Row 2: Go To End, Restart, Load
    goToEnd.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);
    buttonX += buttonWidth + padding;
    restartButton.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);
    buttonX += buttonWidth + padding;
    loadButton.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);

    yPosition += buttonHeight + padding; 
    buttonX = xPosition;                

        // Button Row 3: Mute, Loop
    muteButton.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);
    buttonX += buttonWidth + padding;
    loopButton.setBounds(buttonX, yPosition, buttonWidth, buttonHeight);
}

//==============================================================================

// Buttons Logic
void PlayerGUI::buttonClicked(juce::Button* button){
    if (button == &loadButton) {
        fileChooser = std::make_unique<juce::FileChooser>("Select audio files...",
            juce::File{},
            "*.wav;*.mp3;*.aiff");

        auto folderChooserFlags = juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::canSelectMultipleItems;

        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                auto results = chooser.getResults();

                for (const auto& file : results){
                    if (file.exists())
                        trackFiles.add(file);
                }
                playlist.updateContent(); 
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

// Playlist Functions
int PlayerGUI::getNumRows(){
    return trackFiles.size();
}
void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected){
    if (rowIsSelected)
        g.fillAll(juce::Colours::blue);

    g.setColour(juce::Colours::white);
    g.drawText(trackFiles[rowNumber].getFileName(),
        5, 0, width - 10, height, 
        juce::Justification::centredLeft, true);
}
void PlayerGUI::listBoxItemClicked(int rowNumber, const juce::MouseEvent& e){
    juce::File fileToPlay = trackFiles[rowNumber];
    juce::String title = playerAudio.loadFile(fileToPlay);
    trackTitle.setText(title, juce::dontSendNotification);
    playerAudio.play();
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
