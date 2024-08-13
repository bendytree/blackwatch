#pragma once

#include <juce_core/juce_core.h>
#include "IGuiModel.h";

struct IAppSettings
{
  IGuiModel gui;

  void assign(const IAppSettings& other);

  static IAppSettings current;

  void save();

  static IAppSettings from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;

};
