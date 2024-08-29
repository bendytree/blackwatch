#include "./MainAudio.h"
#include "./MainGui.h"
#include "RmsChangedEvent.h"
#include "BwLogger.h"

MainAudio::MainAudio()
    : rms(4000), AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) {
  midiProcessor = std::make_unique<MyMidiProcessor>();
}

MainAudio::~MainAudio() {}

const juce::String MainAudio::getName() const {
  return JucePlugin_Name;
}

bool MainAudio::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool MainAudio::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool MainAudio::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double MainAudio::getTailLengthSeconds() const {
  return 0.0;
}

int MainAudio::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int MainAudio::getCurrentProgram() {
  return 0;
}

void MainAudio::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String MainAudio::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void MainAudio::changeProgramName(int index,
                                                  const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void MainAudio::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need...
  juce::ignoreUnused(sampleRate, samplesPerBlock);

  int numChannels;
  if (auto* processor = dynamic_cast<juce::AudioProcessor*>(this)) {
    numChannels = processor->getMainBusNumOutputChannels();
  } else {
    numChannels = 2; // Default to stereo
  }

  mySynth.prepareToPlay(sampleRate, samplesPerBlock, numChannels);
}

void MainAudio::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool MainAudio::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void MainAudio::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {

  juce::ScopedNoDenormals noDenormals;
//
  // Clear the buffer
  buffer.clear();

  juce::MidiMessage m;
  int time;
  juce::MidiBuffer::Iterator it(midiMessages);
  while (it.getNextEvent(m, time)) {
    if (m.isNoteOn()) {
      mySynth.synth.noteOn(m.getChannel(), m.getNoteNumber(), m.getFloatVelocity());
    } else if (m.isNoteOff()) {
      mySynth.synth.noteOff(m.getChannel(), m.getNoteNumber(), m.getFloatVelocity(), true);
    }
  }

  // Render audio from your synth into the buffer
  //mySynth.synth.renderNextBlock(buffer, juce::MidiBuffer(), 0, buffer.getNumSamples());
  mySynth.renderNextBlock(buffer);

  midiMessages.clear();

  // Calculate RMS every 100ms
  static int sampleCounter = 0;
  static float perMs = 100;
  static float msPerSecond = 1000;
  static int samplesPerLog = static_cast<int>((perMs / msPerSecond) * getSampleRate());
  sampleCounter += buffer.getNumSamples();
  rms.addBuffer(buffer);
  if (sampleCounter >= samplesPerLog) {
    float leftRMS = rms.getLeftRMS();
    bool isBufferMono = (buffer.getNumChannels() == 1);

    float rightRMS = isBufferMono ? leftRMS : rms.getRightRMS();

//    BwLogger::log("RMS: mono: "+juce::String(isBufferMono ? "true" : "false"));
//    BwLogger::log("RMS: "+juce::String(leftRMS)+", "+juce::String(rightRMS));

    RmsChangedEvent::trigger(leftRMS, rightRMS);

    // Reset the counter
    sampleCounter = 0;
  }
}

bool MainAudio::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MainAudio::createEditor() {
  BwLogger::log("createEditor");
  return new MainGui(*this);
}

void MainAudio::getStateInformation(
    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void MainAudio::setStateInformation(const void* data,
                                                    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new MainAudio();
}
