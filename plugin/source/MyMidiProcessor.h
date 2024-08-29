
#pragma once

#include "juce_audio_devices/juce_audio_devices.h"

class MyMidiProcessor : public juce::MidiInputCallback, private juce::Timer
{
public:
  MyMidiProcessor();
  ~MyMidiProcessor();

  void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message);

private:
  juce::OwnedArray<juce::MidiInput> midiInputs;
  juce::Array<juce::MidiDeviceInfo> lastMidiInputsInfo;

  void timerCallback() override;
  void refreshMidiInputs();
};
