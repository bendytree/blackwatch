
#pragma once

#include <juce_core/juce_core.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

struct IRepoSampleSound
{
  juce::String filename;
  int note;

  bool exists() const;
  std::unique_ptr<juce::MemoryBlock> getMemoryBlock() const;
  juce::File getFile() const;

  static IRepoSampleSound from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;
};

struct IRepoSample
{
  std::string id;
  std::string title;
  std::vector<IRepoSampleSound> sounds;

  static IRepoSample from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;
};

struct IRepo
{
  int version;
  std::vector<IRepoSample> samples;

  static juce::File getAudioFileDir();
  static IRepoSample getSampleById(const std::string& sampleId);

  static IRepo from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;

  static IRepo current(); // Singleton
};
