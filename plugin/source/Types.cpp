#include "Types.h"
#include "Repo.h"

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

IGuiModel IGuiModel::getDefault() {
  IGuiModel d =  {
    sampleId: IRepo::current.samples[0].id,
    attack: 0.1,
    decay: 0.1,
    sustain: 1,
    release: 0.1,
    hpFreq: 10,
    hpResonance: 0.707,
    lpFreq: 30000,
    lpResonance: 0.707,
    modMix: 0.5,
    modDepth: 0.1,
    modRate: 1,
  };
  return d;
}

IGuiModel IGuiModel::current = {
  attack: 1,
  decay: 2,
  sustain: 3,
  release: 4,
};

