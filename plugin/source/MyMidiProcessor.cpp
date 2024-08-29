//
// Created by Josh Wright on 8/28/24.
//
#pragma once

#include "MyMidiProcessor.h"
#include "BwLogger.h"
#include "MainGui.h"


MyMidiProcessor::MyMidiProcessor()
{
  refreshMidiInputs();
  startTimer(1000); // Check every second for MIDI device changes
}

MyMidiProcessor::~MyMidiProcessor()
{
  stopTimer();
}

void MyMidiProcessor:: handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
  if (message.isController())
  {
    int controllerNumber = message.getControllerNumber();
    int controllerValue = message.getControllerValue();

    // IDI CC - Controller: 93, Value: 127
    BwLogger::log("MIDI - Controller: " + source->getDeviceInfo().name + juce::String(controllerNumber) + ", Value: " + juce::String(controllerValue));
    MidiChangedEvent::trigger(controllerNumber, controllerValue, source->getDeviceInfo().name.toStdString());
  }else{
    //BwLogger::log("MIDI CC - NOT Controller");
  }
}

void MyMidiProcessor::timerCallback()
{
  auto currentMidiInputsInfo = juce::MidiInput::getAvailableDevices();
  if (currentMidiInputsInfo != lastMidiInputsInfo)
  {
    BwLogger::log("MIDI input devices list changed");
    refreshMidiInputs();
  }
}

void MyMidiProcessor::refreshMidiInputs()
{
  midiInputs.clear(); // Clear existing MIDI inputs
  lastMidiInputsInfo = juce::MidiInput::getAvailableDevices();

  if (lastMidiInputsInfo.isEmpty())
  {
    BwLogger::log("No MIDI input devices available");
  }
  else
  {
    for (const auto& midiInputInfo : lastMidiInputsInfo)
    {
      std::unique_ptr<juce::MidiInput> midiInput = juce::MidiInput::openDevice(midiInputInfo.identifier, this);

      if (midiInput != nullptr)
      {
        midiInput->start();
        BwLogger::log("MIDI input device opened: " + midiInputInfo.name);
        midiInputs.add(midiInput.release());
      }
      else
      {
        BwLogger::log("Could not open MIDI input device: " + midiInputInfo.name);
      }
    }
  }
}
