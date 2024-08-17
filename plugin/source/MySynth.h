#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_dsp/juce_dsp.h"

class MySynth {
public:
    MySynth();
    void reload();
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples, juce::MidiBuffer& midiMessages);

    juce::Synthesiser synth;

  private:
    void updateSettings();

    //    juce::dsp::LadderFilter<float>& lpFilter;
//    juce::dsp::LadderFilter<float>& hpFilter;
//    juce::dsp::Chorus<float>& chorusFilter;
//    juce::dsp::ProcessorChain<
//        juce::dsp::LadderFilter<float>,
//        juce::dsp::LadderFilter<float>,
//        juce::dsp::Chorus<float>
//    > effectsChain;
};
