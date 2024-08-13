#include "IGuiModel.h"
#include "Repo.h"

IGuiModel IGuiModel::from_json(const juce::String& jsonString) {
  auto json = nlohmann::json::parse(jsonString.toStdString());
  IGuiModel model;
  model.sampleId = json.at("sampleId").get<std::string>();
  model.attack = json.at("attack").get<float>();
  model.decay = json.at("decay").get<float>();
  model.sustain = json.at("sustain").get<float>();
  model.release = json.at("release").get<float>();
  model.hpFreq = json.at("hpFreq").get<float>();
  model.hpResonance = json.at("hpResonance").get<float>();
  model.lpFreq = json.at("lpFreq").get<float>();
  model.lpResonance = json.at("lpResonance").get<float>();
  model.modDepth = json.at("modDepth").get<float>();
  model.modMix = json.at("modMix").get<float>();
  model.modRate = json.at("modRate").get<float>();
  return model;
}

juce::String IGuiModel::to_json() const {
  nlohmann::json json;
  json["sampleId"] = sampleId;
  json["attack"] = attack;
  json["decay"] = decay;
  json["sustain"] = sustain;
  json["release"] = release;
  json["hpFreq"] = hpFreq;
  json["hpResonance"] = hpResonance;
  json["lpFreq"] = lpFreq;
  json["lpResonance"] = lpResonance;
  json["modDepth"] = modDepth;
  json["modMix"] = modMix;
  json["modRate"] = modRate;
  return juce::String(json.dump());
}

void IGuiModel::assign(const IGuiModel& other) {
  sampleId = other.sampleId;
  attack = other.attack;
  decay = other.decay;
  sustain = other.sustain;
  release = other.release;
  hpFreq = other.hpFreq;
  hpResonance = other.hpResonance;
  lpFreq = other.lpFreq;
  lpResonance = other.lpResonance;
  modDepth = other.modDepth;
  modMix = other.modMix;
  modRate = other.modRate;
}

IGuiModel IGuiModel::getDefault() {
  IGuiModel d =  {
    sampleId: IRepo::current().samples[0].id,
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


