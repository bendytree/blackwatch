#pragma once

#include "MainAudio.h"
#include "Browser.h"
#include <juce_gui_extra/juce_gui_extra.h>


class MainGui : public juce::AudioProcessorEditor {
public:
  explicit MainGui(MainAudio&);
  ~MainGui() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  MainAudio& mainAudio;
  std::unique_ptr<BrowserComponent> webBrowser;
  std::unique_ptr<juce::ComponentBoundsConstrainer> constrainer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainGui)
};
