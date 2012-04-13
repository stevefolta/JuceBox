/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JuceBoxAudioProcessorEditor::JuceBoxAudioProcessorEditor (JuceBoxAudioProcessor* ownerFilter)
	: AudioProcessorEditor(ownerFilter),
	button("Load..."),
	pathLabel(String::empty),
	midiKeyboard(ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
	// This is where our plugin's editor size is set.
	setSize (500, 300);

	addAndMakeVisible(&button);
	button.addListener(this);

	addAndMakeVisible(&pathLabel);
	pathLabel.setFont(Font(14.0));

	addAndMakeVisible(&midiKeyboard);
}


JuceBoxAudioProcessorEditor::~JuceBoxAudioProcessorEditor()
{
}


void JuceBoxAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::white);
}


void JuceBoxAudioProcessorEditor::resized()
{
	enum {
		hMargin = 10,
		vMargin = 4,
		buttonHeight = 25,
		pathHeight = 25,
		keyboardHeight = 70,
		};
	button.setBounds(hMargin, vMargin, 150, buttonHeight);
	pathLabel.setBounds(
		hMargin, vMargin + buttonHeight, getWidth() - 2 * hMargin, pathHeight);
	midiKeyboard.setBounds(
		hMargin, getHeight() - keyboardHeight - vMargin,
		getWidth() - 2 * hMargin, keyboardHeight);
}


void JuceBoxAudioProcessorEditor::buttonClicked(Button* clickedButton)
{
	if (clickedButton == &button) {
		FileChooser chooser(
			"Select a sample file...",
			File::nonexistent,
			"*.wav;*.WAV;*.ogg;*.AIF;*.aiff");
		if (chooser.browseForFileToOpen()) {
			File sampleFile(chooser.getResult());
			pathLabel.setText(sampleFile.getFullPathName(), false);
			}
		}
}




