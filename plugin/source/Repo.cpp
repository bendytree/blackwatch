
#include "Repo.h"
#include "BinaryData.h"

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
    sounds.push_back(IRepoSampleSound::from_json(juce::String(soundJson.dump())));
  }
  return {json["id"].get<std::string>(), json["title"].get<std::string>(), sounds};
}

juce::String IRepoSample::to_json() const
{
  nlohmann::json json;
  json["id"] = id;
  json["title"] = title;
  for (const auto& sound : sounds)
  {
    json["sounds"].push_back(nlohmann::json::parse(sound.to_json().toStdString()));
  }
  return juce::String(json.dump());
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
  for (const auto& sample : samples)
  {
    json["samples"].push_back(nlohmann::json::parse(sample.to_json().toStdString()));
  }
  return juce::String(json.dump());
}


juce::String json = juce::String(juce::CharPointer_UTF8(BinaryData::repo_json), BinaryData::repo_jsonSize);
IRepo IRepo::current = IRepo::from_json(json);

