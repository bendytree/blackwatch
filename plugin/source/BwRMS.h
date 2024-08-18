#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"
#include <vector>

class BwRMS
{
public:
  BwRMS(int numSamples);

  void addBuffer(const juce::AudioBuffer<float>& buffer);

  float getLeftRMS() const;
  float getRightRMS() const;

private:
  void calculateRMS();

  int maxSamples;
  std::vector<float> leftChannelSamples;
  std::vector<float> rightChannelSamples;

  float leftRMS {0.0f};
  float rightRMS {0.0f};
};
