#include "Types.h"

IGuiModel IGuiModel::from_json(const juce::String& jsonString) {
  auto json = nlohmann::json::parse(jsonString.toStdString());
  IGuiModel model;
  model.attack = json.at("attack").get<float>();
  model.decay = json.at("decay").get<float>();
  model.sustain = json.at("sustain").get<float>();
  model.release = json.at("release").get<float>();
  return model;
}

juce::String IGuiModel::to_json() const {
  nlohmann::json json;
  json["attack"] = attack;
  json["decay"] = decay;
  json["sustain"] = sustain;
  json["release"] = release;
  return juce::String(json.dump());
}

void IGuiModel::assign(const IGuiModel& other) {
  attack = other.attack;
  decay = other.decay;
  sustain = other.sustain;
  release = other.release;
}

IGuiModel IGuiModel::current = {
  attack: 1,
  decay: 2,
  sustain: 3,
  release: 4,
};

