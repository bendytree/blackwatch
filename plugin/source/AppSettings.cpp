#include "AppSettings.h"

static juce::File getSettingsFile()
{
  return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("BlackWatchStudios/settings.json");
}

static IAppSettings getDefaultSettings()
{
  IAppSettings settings;
  settings.gui = IGuiModel::getDefault();
  return settings;
}

static IAppSettings loadSettings()
{
  juce::File settingsFile = getSettingsFile();
  if (settingsFile.existsAsFile())
  {
    try {
      auto json = settingsFile.loadFileAsString();
      return IAppSettings::from_json(json);
    }catch (const std::exception& e){}
  }
  return getDefaultSettings();
}

void IAppSettings::save()
{
  std::string json = this->to_json().toStdString();
  juce::File settingsFile = getSettingsFile();
  if (!settingsFile.getParentDirectory().exists())
  {
    settingsFile.getParentDirectory().createDirectory();
  }
  settingsFile.replaceWithText(json);
}

IAppSettings IAppSettings::from_json(const juce::String& jsonString)
{
  auto obj = nlohmann::json::parse(jsonString.toStdString());
  juce::String guiJson = juce::String(obj["gui"].dump());
  auto gui = IGuiModel::from_json(guiJson);
  return { gui };
}

juce::String IAppSettings::to_json() const
{
  nlohmann::json json;
  json["gui"] = nlohmann::json::parse(gui.to_json().toStdString());
  return juce::String(json.dump());
}

void IAppSettings::assign(const IAppSettings& other) {
  gui = other.gui;
}


IAppSettings IAppSettings::current = loadSettings();
