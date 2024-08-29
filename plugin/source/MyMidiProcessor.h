
#pragma once

#include "juce_audio_devices/juce_audio_devices.h"

class MyMidiProcessor : juce::MidiInputCallback, private juce::Timer
{
public:
  MyMidiProcessor();
  ~MyMidiProcessor() override;

  void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

private:
  juce::OwnedArray<juce::MidiInput> midiInputs;
};
