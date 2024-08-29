#pragma once

#include <string>
#include <juce_audio_processors/juce_audio_processors.h>

class SettingsRepo
{
public:
  // Constructor
  explicit SettingsRepo(juce::AudioProcessor& processor);

  // Save the settings
  void save(const std::string& settings);

  // Load the settings
  std::string load() const;

  // Handle state information for the processor
  void getStateInformation(juce::MemoryBlock& destData);
  void setStateInformation(const void* data, int sizeInBytes);

private:
  juce::AudioProcessor& processor;
  juce::AudioProcessorValueTreeState valueTreeState;

  // Helper function to create parameter layout
  juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
