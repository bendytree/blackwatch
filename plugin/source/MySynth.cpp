#include "MySynth.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "PubSub.h"
#include "AppSettings.h"
#include "Repo.h"
#include "BwLogger.h"

MySynth::MySynth()
//    :
//      lpFilter(effectsChain.get<0>()),
//      hpFilter(effectsChain.get<1>()),
//      chorusFilter(effectsChain.get<2>())
{
    synth.clearSounds();

    for (int i = 0; i < 64; ++i) { // Notes at a time
        synth.addVoice(new juce::SamplerVoice());
    }

    reload();

    PubSub::shared().on(PubSubEvent::GuiModelSampleChanged, [this]() {
      BwLogger::log("PubSub.GuiModelSampleChanged");
      reload();
    });

    PubSub::shared().on(PubSubEvent::GuiModelSettingsChanged, [this]() {
      BwLogger::log("PubSub.GuiModelSettingsChanged");
      updateSettings();
    });
}

void MySynth::reload() {
    synth.clearSounds();

    BwLogger::log("MySynth.reload...");

    auto& settings = IAppSettings::current;
    std::string sampleId;
    sampleId = settings.gui.sampleId;
    BwLogger::log("MySynth.sampleId: "+sampleId);
    if (sampleId.empty()) {
      BwLogger::log("MySynth.sampleId is empty");
      throw std::runtime_error( "sampleId is empty" );
      return;
    }
    auto samples = IRepo::current().samples;
    BwLogger::log("MySynth.samples: "+ juce::String(samples.size()));
    auto sample = IRepo::getSampleById(sampleId); // Throws
    //BwLogger::log("MySynth.sample is: "+sample.to_json());
    BwLogger::log("MySynth.sample.sounds: "+ juce::String(sample.sounds.size()));

    // Filter by existence
    std::vector<IRepoSampleSound> sounds;
    std::copy(sample.sounds.begin(), sample.sounds.end(), std::back_inserter(sounds));
    BwLogger::log("MySynth.sounds: "+juce::String(sounds.size()));

    // Sort by note
    std::sort(sounds.begin(), sounds.end(), [](const IRepoSampleSound& a, const IRepoSampleSound& b) {
        return a.note < b.note;
    });

    int nextStartNote = 0;
    for (size_t i = 0; i < sounds.size(); ++i) {

        int startNote = nextStartNote;
        int endNote = 127;
        auto sound = sounds[i];
        auto exists = sound.exists();
        BwLogger::log("MySynth.sound["+juce::String(i)+"].exists = "+(exists ? "true" : "false"));
        if (!exists) {
          throw std::runtime_error( "sound does not exist" );
          continue;
        }

        if (i < sounds.size() - 1) {
            endNote = static_cast<int>(std::ceil((sound.note + sounds[i + 1].note) / 2));
            nextStartNote = endNote + 1;
        }

        juce::BigInteger midiNotes;
        midiNotes.setRange(startNote, endNote, true); // Assumes you want to map this sound across all MIDI notes, adjust if needed

        auto memoryBlock = sound.getMemoryBlock();
        auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(*memoryBlock, false);
        if (memoryInputStream == nullptr) {
            throw std::runtime_error("memoryInputStream is null for " + sound.filename.toStdString());
        }

        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::AudioFormatReader> formatReader(wavFormat.createReaderFor(memoryInputStream.release(), true));
        if (formatReader == nullptr) {
            throw std::runtime_error("formatReader is null for " + sound.filename.toStdString());
        }
        double maxSeconds = static_cast<double>(formatReader->lengthInSamples) / formatReader->sampleRate;

        auto* synthSound = new juce::SamplerSound(sample.title,
                                             *formatReader,
                                             midiNotes,
                                             sound.note,   // root midi note
                                             settings.gui.attack,  // attack time in seconds
                                             settings.gui.release,  // release time in seconds
                                             maxSeconds  // max sample length in seconds

        );
        BwLogger::log("MySynth.addSound!");
        synth.addSound(synthSound);
    }

    updateSettings();

    BwLogger::log("MySynth.synth.getNumSounds: " + juce::String(MySynth::synth.getNumSounds()));
    BwLogger::log("MySynth.synth.getNumVoices: " + juce::String(MySynth::synth.getNumVoices()));
}

void MySynth::updateSettings() {
  BwLogger::log("MySynth.updateSettings...");
  return;

  // Effects
  auto& settings = IAppSettings::current;
  BwLogger::log("MySynth.settings: " + settings.to_json());
//  lpFilter.setMode(juce::dsp::LadderFilter<float>::Mode::LPF12);
//  lpFilter.setCutoffFrequencyHz(settings.gui.lpFreq);
//  lpFilter.setResonance(settings.gui.lpResonance);
//
//  hpFilter.setMode(juce::dsp::LadderFilter<float>::Mode::HPF12);
//  hpFilter.setCutoffFrequencyHz(settings.gui.hpFreq);
//  hpFilter.setResonance(settings.gui.hpResonance);
//
//  chorusFilter.setRate(settings.gui.modRate);
//  chorusFilter.setDepth(settings.gui.modDepth);
//  chorusFilter.setMix(settings.gui.modMix);

  auto* sound = dynamic_cast<juce::SamplerSound*>(synth.getSound(0).get());
  if (sound != nullptr) {
    juce::ADSR::Parameters adsr;
    adsr.attack = settings.gui.attack;
    adsr.decay = settings.gui.decay;
    adsr.sustain = settings.gui.sustain;
    adsr.release = settings.gui.release;
    sound->setEnvelopeParameters(adsr);
  }

}


void MySynth::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples, juce::MidiBuffer& midiMessages) {

  // Render the sound


//    // Clear MIDI messages after processing
//    midiMessages.clear();

//  // Effects
//  juce::dsp::AudioBlock<float> audioBlock(outputBuffer);
//  juce::dsp::ProcessContextReplacing<float> context(audioBlock);
//  effectsChain.process(context);
}
