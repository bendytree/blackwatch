#ifndef TYPES_H
#define TYPES_H

#include <nlohmann/json.hpp>
#include <juce_core/juce_core.h>

struct IGuiModel {
  float attack;
  float decay;
  float sustain;
  float release;

  void assign(const IGuiModel& other);

  static IGuiModel current;

  static IGuiModel from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;
};

#endif // TYPES_H
