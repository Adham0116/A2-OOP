#include "PlayerGUI.h"

//==============================================================================
// Constructor: This is where you initialize your GUI components.
PlayerGUI::PlayerGUI()
// This is the correct C++ syntax for an initializer list.
  : loadButton("Load"),
    playButton("Play"),
    stopButton("Stop"),
    restartButton("Restart")
{
    // Make the buttons and slider visible on the screen.
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(gainSlider);
	addAndMakeVisible(restartButton);
	addAndMakeVisible(loopButton);

    // Register this class to listen for clicks and slider movements.
    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    gainSlider.addListener(this);
    loopButton.addListener(this);
    restartButton.addListener(this);
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

// Destructor: Clean up listeners.
PlayerGUI::~PlayerGUI()
{
    loadButton.removeListener(this);
    playButton.removeListener(this);
    stopButton.removeListener(this);
    gainSlider.removeListener(this);
    restartButton.removeListener(this);
    loopButton.removeListener(this);
}

//==============================================================================
void PlayerGUI::paint(juce::Graphics& g)
{
    // Fill the background with a dark grey color.
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    // This defines the layout of your GUI components.
    int padding = 10;
    int buttonWidth = 100;
    loadButton.setBounds(padding, padding, 100, 30);
    playButton.setBounds(loadButton.getRight() + padding, padding, 100, 30);
    stopButton.setBounds(playButton.getRight() + padding, padding, 100, 30);
    gainSlider.setBounds(padding, loadButton.getBottom() + padding, getWidth() - 2 * padding, 30);
    restartButton.setBounds(stopButton.getRight() + padding, padding, buttonWidth, 30);
    loopButton.setBounds(restartButton.getRight() + padding, padding, buttonWidth, 30);
}

//==============================================================================
// This function is called when any button is clicked.
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        // Open a file dialog to choose an audio file.
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.aiff");

        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        // Launch the dialog asynchronously.
        fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File result = chooser.getResult();
                if (result.exists())
                {
                    // If a file was chosen, load it into the audio player.
                    playerAudio.loadFile(result);
                }
            });
    }
    else if (button == &playButton)
    {
        // Tell the audio player to start playing.
        playerAudio.play();
    }
    else if (button == &stopButton)
    {
        // Tell the audio player to stop.
        playerAudio.stop();
    }
    else if (button == &restartButton) { playerAudio.restart(); }

    // *** STEP 4: HANDLE CLICKS ON THE LOOP BUTTON ***
    else if (button == &loopButton)
    {
        // getToggleState() returns true if the button is "on" and false if it's "off".
        // We pass this state directly to our audio engine.
        playerAudio.setLooping(loopButton.getToggleState());
    }
}

// This function is called when the slider's value changes.
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        // Set the gain on the audio player.
        playerAudio.setGain((float)gainSlider.getValue());
    }
}

//==============================================================================
// These are the "pass-through" methods that connect MainComponent to PlayerAudio.
//==============================================================================

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