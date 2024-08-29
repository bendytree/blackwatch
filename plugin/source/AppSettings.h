#pragma once

#include <juce_core/juce_core.h>
#include "IGuiModel.h";

struct IAppSettings
{
  IGuiModel gui;


  static void assignCurrent(const IAppSettings& settings);
  static IAppSettings getCurrent();

  static IAppSettings from_json(const juce::String& jsonString);
  [[nodiscard]] juce::String to_json() const;

private:
  void assign(const IAppSettings& other);
  static IAppSettings current;
  static std::mutex settingsMutex;
};
