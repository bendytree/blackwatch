
#pragma once

#include <juce_core/juce_core.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

struct IRepoSampleSound
{
  juce::String filename;
  int note;

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

  static IRepo from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;

  static IRepo current; // Singleton
};
