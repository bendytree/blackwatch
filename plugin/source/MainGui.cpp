

#include "./MainGui.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "Browser.h"

namespace audio_plugin {
MainGui::MainGui(MainAudio& p) : juce::AudioProcessorEditor (&p), mainAudio(p)
{
  webBrowser = std::make_unique<BrowserComponent>();


  juce::ignoreUnused(mainAudio);

  addAndMakeVisible(*webBrowser);

  // Set the initial size of the editor
  setSize(800, 600);


}

MainGui::~MainGui()
{

}

void MainGui::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainGui::resized() {
  webBrowser->setBounds(getLocalBounds());
}


}  // namespace audio_plugin
