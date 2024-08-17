
#include "Repo.h"
#include "BinaryData.h"
#include "BwLogger.h"

IRepoSampleSound IRepoSampleSound::from_json(const juce::String& jsonString)
{
  auto json = nlohmann::json::parse(jsonString.toStdString());
  return {json["filename"].get<std::string>(), json["note"].get<int>()};
}

juce::String IRepoSampleSound::to_json() const
{
  nlohmann::json json;
  json["filename"] = filename.toStdString();
  json["note"] = note;
  return juce::String(json.dump());
}

IRepoSample IRepoSample::from_json(const juce::String& jsonString)
{
  auto json = nlohmann::json::parse(jsonString.toStdString());
  std::vector<IRepoSampleSound> sounds;
  for (const auto& soundJson : json["sounds"])
  {
    sounds.push_back(IRepoSampleSound::from_json(juce::String(soundJson.dump(0))));
  }
  IRepoSample sample = { json["id"].get<std::string>(), json["title"].get<std::string>(), sounds };
  return sample;
}


juce::String IRepoSample::to_json() const
{
  nlohmann::json json;
  json["id"] = id;
  json["title"] = title;
  json["sounds"] = nlohmann::json::array();
  for (const auto& sound : sounds)
  {
    json["sounds"].push_back(nlohmann::json::parse(sound.to_json().toStdString()));
  }
  return juce::String(json.dump());
}


juce::File IRepo::getAudioFileDir() {
  return juce::File("/Users/josh/Library/BlackWatchStudios/wavs/");
//  juce::File appDataDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);
//  juce::File blackwatchDir = appDataDir.getChildFile("blackwatch");
//  if (!blackwatchDir.exists()) {
//    bool dirCreated = blackwatchDir.createDirectory();
//    if (!dirCreated) {
//      DBG("Failed to create the blackwatch directory.");
//    }
//  }
//  return blackwatchDir;
}

juce::File IRepoSampleSound::getFile() const {
  auto file = IRepo::getAudioFileDir().getChildFile(filename);
  return juce::File(file);
}


bool IRepoSampleSound::exists() const {
  if (getFile().exists()) {
    return true;
  }
  return false;
}

std::unique_ptr<juce::MemoryBlock> IRepoSampleSound::getMemoryBlock() const {
  auto file = getFile();
  if (!file.exists()) {
    return nullptr;
  }

  std::unique_ptr<juce::FileInputStream> fileInputStream(file.createInputStream());
  if (fileInputStream == nullptr) { return nullptr; }

  auto memoryBlock = std::make_unique<juce::MemoryBlock>();
  fileInputStream->readIntoMemoryBlock(*memoryBlock);
  return memoryBlock;
}

IRepoSample IRepo::getSampleById(const std::string& sampleId) {
  for (auto& sample : IRepo::current().samples) {
    if (sample.id == sampleId) {
      BwLogger::log("find.sounds: "+juce::String(sample.sounds.size()));
      return sample;
    }
  }
  throw std::runtime_error("sampleId not found: " + std::string(sampleId));
}

IRepo IRepo::from_json(const juce::String& jsonString)
{
  auto json = nlohmann::json::parse(jsonString.toStdString());
  std::vector<IRepoSample> samples;
  for (const auto& sampleJson : json["samples"])
  {
    samples.push_back(IRepoSample::from_json(juce::String(sampleJson.dump())));
  }
  return {json["version"].get<int>(), samples};
}

juce::String IRepo::to_json() const
{
  nlohmann::json json;
  json["version"] = version;
  json["samples"] = nlohmann::json::array();
  for (const auto& sample : samples)
  {
    json["samples"].push_back(nlohmann::json::parse(sample.to_json().toStdString()));
  }
  return juce::String(json.dump());
}

IRepo IRepo::current() {
  static IRepo instance = []() {
    BwLogger::log("Repo.init!");
    juce::String json = juce::String(juce::CharPointer_UTF8(BinaryData::repo_json), BinaryData::repo_jsonSize);
    auto r = IRepo::from_json(json);
    return r;
  }();
  return instance;
}


