#include "BwRMS.h"

BwRMS::BwRMS(int numSamples) : maxSamples(numSamples)
{
}

void BwRMS::addBuffer(const juce::AudioBuffer<float>& buffer)
{
  int numSamples = buffer.getNumSamples();
  bool isMono = buffer.getNumChannels() <= 1;

  for (int i = 0; i < numSamples; ++i)
  {
    if (leftChannelSamples.size() >= maxSamples)
    {
      leftChannelSamples.erase(leftChannelSamples.begin());
    }
    if (rightChannelSamples.size() >= maxSamples) {
      rightChannelSamples.erase(rightChannelSamples.begin());
    }

    leftChannelSamples.push_back(buffer.getReadPointer(0)[i]);

    if (!isMono) {
      rightChannelSamples.push_back(buffer.getReadPointer(1)[i]);
    }
  }

  calculateRMS();
}

void BwRMS::calculateRMS()
{
  float leftSumSquares = 0.0f;
  float rightSumSquares = 0.0f;

  for (float sample : leftChannelSamples)
    leftSumSquares += abs(sample);

  for (float sample : rightChannelSamples)
    rightSumSquares += abs(sample);

  float lSize = leftChannelSamples.size();
  float rSize = rightChannelSamples.size();

  leftRMS = lSize == 0 ? 0 : std::pow(leftSumSquares / lSize, 1.0f/4.0f);
  rightRMS = rSize == 0 ? 0 : std::pow(rightSumSquares / rSize, 1.0f/4.0f);
}

float BwRMS::getLeftRMS() const
{
  return leftRMS;
}

float BwRMS::getRightRMS() const
{
  return rightRMS;
}
