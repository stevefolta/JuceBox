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
	blocksStarted = blocksFinished = 0;

	// Accept *all* the formats Juce supports.
	// Why doesn't AudioFormatManager have a function for this?
	formatManager.registerFormat(new AiffAudioFormat(), false);
	formatManager.registerFormat(new WavAudioFormat(), false);
#if JUCE_USE_OGGVORBIS
	formatManager.registerFormat(new OggVorbisAudioFormat(), false);
#endif
#if JUCE_USE_FLAC
	formatManager.registerFormat(new FlacAudioFormat(), false);
#endif
#if JUCE_USE_MP3AUDIOFORMAT
	formatManager.registerFormat(new MP3AudioFormat(), false);
#endif
#if JUCE_MAC || JUCE_IOS
	formatManager.registerFormat(new CoreAudioFormat(), false);
#endif
#if JUCE_QUICKTIME
	formatManager.registerFormat(new QuickTimeAudioFormat(), false);
#endif
#if JUCE_WINDOWS
	formatManager.registerFormat(new WindowsMediaAudioFormat(), false);
#endif

	for (int i = 0; i < 16; ++i)
		synth.addVoice(new SamplerVoice());
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
	loadSound();
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
	synth.setCurrentPlaybackSampleRate(sampleRate);
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
	blocksStarted += 1;

	int numSamples = buffer.getNumSamples();

	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

	synth.renderNextBlock(buffer, midiMessages, 0, numSamples);

	blocksFinished += 1;
}


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
			loadSound();
			}
		}
}


void JuceBoxAudioProcessor::loadSound()
{
	Logger::writeToLog("- loadSound().");
	synth.clearSounds();

	if (!sampleFile.existsAsFile()) {
		Logger::writeToLog("\"" + sampleFile.getFullPathName() + "\" doesn't exist.");
		return;
		}
	AudioFormatReader* reader = formatManager.createReaderFor(sampleFile);
	if (reader == NULL) {
		Logger::writeToLog("No reader for \"" + sampleFile.getFullPathName() + "\".");
		return;
		}
	Logger::writeToLog("Format: " + reader->getFormatName());
	Logger::writeToLog("Sample rate: " + String(reader->sampleRate));
	Logger::writeToLog("length: " + String(reader->lengthInSamples));
	Logger::writeToLog("numChannels: " + String(reader->numChannels));
	BigInteger notes;
	notes.setRange(0, 127, true);
	SamplerSound* sound =
		new SamplerSound(
			sampleFile.getFileNameWithoutExtension(),
			*reader,
			notes,
			72 /* C5 == middle C above A-440 */,
			0.0, 0.0,
			20.0 /* max time, hopefully 20s is enough for any sound */);
	synth.addSound(sound);
	delete reader;
}


String JuceBoxAudioProcessor::formatWildcards()
{
	return formatManager.getWildcardForAllFormats();
}


String JuceBoxAudioProcessor::infoString()
{
	SamplerSound* sound = getSound();
	if (sound == NULL)
		return "";
	return
		String(sound->lastNote) + (sound->lastNoteOn ? " on" : " off") +
		" proc: " +
		String(blocksStarted) + "/" + String(blocksFinished) +
		" midi: " + String(synth.midiEvents) +
		" notes: " +
		String(synth.notesOn) + "/" + String(synth.notesOff) +
		" synth: " +
		String(synth.blocksStarted) + "/" + String(synth.blocksFinished) +
		" notes: " +
		String(sound->notesApplied) + "/" + String(sound->notesAccepted) +
		" snd: " +
		String((int64) sound->blocksStarted) + "/" +
		String((int64) sound->blocksFinished);
}


SamplerSound* JuceBoxAudioProcessor::getSound()
{
	SynthesiserSound* sound = synth.getSound(0);
	return dynamic_cast<SamplerSound*>(sound);
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceBoxAudioProcessor();
}
