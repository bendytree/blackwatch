#ifndef TYPES_H
#define TYPES_H

#include <nlohmann/json.hpp>
#include <juce_core/juce_core.h>

struct IGuiModel {
  std::string sampleId;
  float attack; //seconds
  float decay; //seconds
  float sustain; //0 through 1
  float release; //seconds
  float hpFreq; // Hz
  float hpResonance; // 0 through 2
  float lpFreq; // Hz
  float lpResonance; // 0 through 2
  float modMix; // 0 through 1
  float modDepth; // 0 through 1
  float modRate; // Hz

  void assign(const IGuiModel& other);

  static IGuiModel current;

  static IGuiModel from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;

  IGuiModel getDefault();
};

#endif // TYPES_H
