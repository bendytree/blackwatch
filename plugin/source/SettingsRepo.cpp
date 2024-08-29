#include "SettingsRepo.h"
#include "BwLogger.h"
#include "MainAudio.h"
#include "AppSettings.h"

// Constructor
SettingsRepo::SettingsRepo(juce::AudioProcessor& proc)
    : processor(proc), valueTreeState(proc, nullptr, "PARAMETERS", createParameterLayout())
{
  // Dont hate, this is the only way to set it before MySynth is initalized. Sorry i suck at cpp
  MainAudio& ma = dynamic_cast<MainAudio&>(proc);
  MainAudio::current = &ma;
  BwLogger::log("SettingsRepo.init");
}

// Create the parameter layout
juce::AudioProcessorValueTreeState::ParameterLayout SettingsRepo::createParameterLayout()
{
  std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
  // Add your parameters here
  return { params.begin(), params.end() };
}

// Save the current state to a memory block
void SettingsRepo::getStateInformation(juce::MemoryBlock& destData)
{
  juce::String json = IAppSettings::getCurrent().to_json();
  // todo: save json to destData
  destData.replaceAll(json.toRawUTF8(), json.getNumBytesAsUTF8());
}

// Restore the state from a memory block
void SettingsRepo::setStateInformation(const void* data, int sizeInBytes)
{
  // todo: deserialize data to a std::string json
  std::string jsona(static_cast<const char*>(data), sizeInBytes);
  auto json = juce::String::fromUTF8(jsona.c_str(), sizeInBytes);
  auto settings = IAppSettings::from_json(json);
  IAppSettings::assignCurrent(settings);
}
