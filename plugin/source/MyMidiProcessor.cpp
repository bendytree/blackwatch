//
// Created by Josh Wright on 8/28/24.
//
#pragma once

#include "MyMidiProcessor.h"
#include "BwLogger.h"
#include "MainGui.h"

MyMidiProcessor::MyMidiProcessor()
{
  auto midiInputs = juce::MidiInput::getAvailableDevices();

  if (midiInputs.isEmpty())
  {
    BwLogger::log("No MIDI input devices available");
  }
  else
  {
    for (const auto& midiInputInfo : midiInputs)
    {
      std::unique_ptr<juce::MidiInput> midiInput = juce::MidiInput::openDevice(midiInputInfo.identifier, this);

      if (midiInput != nullptr)
      {
        midiInput->start();
        BwLogger::log("MIDI input device opened: " + midiInputInfo.name);
        this->midiInputs.add(midiInput.release());
      }
      else
      {
        BwLogger::log("Could not open MIDI input device: " + midiInputInfo.name);
      }
    }
  }
}

void MyMidiProcessor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
  if (message.isController())
  {
    int controllerNumber = message.getControllerNumber();
    int controllerValue = message.getControllerValue();

    // IDI CC - Controller: 93, Value: 127
    BwLogger::log("MIDI - Controller: " + juce::String(controllerNumber) + ", Value: " + juce::String(controllerValue));
    MidiChangedEvent::trigger(controllerNumber, controllerValue);
  }else{
    //BwLogger::log("MIDI CC - NOT Controller");
  }
}

MyMidiProcessor::~MyMidiProcessor()
{
  //  if (midiInput != nullptr)
  //  {
  //    midiInput->stop();
  //  }
}
