/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JuceBoxAudioProcessor::JuceBoxAudioProcessor()
{
	Logger::setCurrentLogger(
		FileLogger::createDefaultAppLogger(
			"JuceBox", "JuceBox.log", "JuceBox started"),
		true);
}

JuceBoxAudioProcessor::~JuceBoxAudioProcessor()
{
}

//==============================================================================
const String JuceBoxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int JuceBoxAudioProcessor::getNumParameters()
{
    return 0;
}

float JuceBoxAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void JuceBoxAudioProcessor::setParameter (int index, float newValue)
{
}


void JuceBoxAudioProcessor::setSampleFile(File* newSampleFile)
{
	sampleFile = *newSampleFile;
}


const String JuceBoxAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String JuceBoxAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String JuceBoxAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String JuceBoxAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool JuceBoxAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool JuceBoxAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool JuceBoxAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool JuceBoxAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int JuceBoxAudioProcessor::getNumPrograms()
{
    return 0;
}

int JuceBoxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceBoxAudioProcessor::setCurrentProgram (int index)
{
}

const String JuceBoxAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void JuceBoxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JuceBoxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	keyboardState.reset();
}

void JuceBoxAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
	keyboardState.reset();
}

void JuceBoxAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	int numSamples = buffer.getNumChannels();

	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	for (int channel = 0; channel < getNumInputChannels(); ++channel) {
		float* channelData = buffer.getSampleData (channel);

		// ..do something to the data...
		}

	// In case we have more outputs than inputs, we'll clear any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
		buffer.clear (i, 0, buffer.getNumSamples());
}

//==============================================================================
bool JuceBoxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JuceBoxAudioProcessor::createEditor()
{
    return new JuceBoxAudioProcessorEditor (this);
}

//==============================================================================
void JuceBoxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	XmlElement xml("JuceBox");
	xml.setAttribute("sampleFile", sampleFile.getFullPathName());
	copyXmlToBinary(xml, destData);
}


void JuceBoxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	Logger::writeToLog("- setStateInformation().");
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState) {
		if (xmlState->hasTagName("JuceBox")) {
			String sampleFilePath = xmlState->getStringAttribute("sampleFile");
			Logger::writeToLog("- sampleFile = " + sampleFilePath);
			sampleFile = sampleFilePath;
			}
		}
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceBoxAudioProcessor();
}
