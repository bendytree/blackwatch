#include "AppSettings.h"
#include "MainAudio.h"
#include "BwLogger.h"

static bool hasLoaded = false;
IAppSettings IAppSettings::current = IAppSettings();
std::mutex IAppSettings::settingsMutex;

static IAppSettings getDefaultSettings()
{
  IAppSettings settings;
  settings.gui = IGuiModel::getDefault();
  return settings;
}

static IAppSettings loadSettings()
{
//  if (MainAudio::current)
//  {
//    BwLogger::log("AppSettings.loadSettings MainAudio::current IS OK");
//    try {
//      auto json = MainAudio::current->settingsRepo.load();
//      BwLogger::log("AppSettings.loadSettings MainAudio::current.load is: "+json);
//      return IAppSettings::from_json(json);
//    }catch (const std::exception& e){}
//  }else{
//    BwLogger::log("AppSettings.loadSettings MainAudio::current IS NULL");
//  }
  return getDefaultSettings();
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
  gui.assign(other.gui);
}

void IAppSettings::assignCurrent(const IAppSettings& settings)
{
  //std::lock_guard<std::mutex> lock(settingsMutex);
  current.assign(settings);
}

IAppSettings IAppSettings::getCurrent()
{
  //std::lock_guard<std::mutex> lock(settingsMutex);
  if (!hasLoaded) {
    hasLoaded = true;
    current.assign(loadSettings());
  }
  BwLogger::log("IAppSettings.getCurrent: "+current.to_json());
  return current;
}
