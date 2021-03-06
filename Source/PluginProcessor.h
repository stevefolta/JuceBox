/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_A44BC18D__
#define __PLUGINPROCESSOR_H_A44BC18D__

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class JuceBoxAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JuceBoxAudioProcessor();
    ~JuceBoxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

		void	setSampleFile(File* newSampleFile);
		File	getSampleFile() { return sampleFile; }

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

		MidiKeyboardState	keyboardState;

		String	formatWildcards();

		String	infoString();
		SamplerSound*	getSound();
		uint64 blocksStarted, blocksFinished;

protected:
		File sampleFile;
		Synthesiser synth;
		AudioFormatManager formatManager;

		void	loadSound();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceBoxAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_A44BC18D__
