

#include "./MainGui.h"
#include "./MainAudio.h"

namespace audio_plugin {
MainGui::MainGui(MainAudio& p) : juce::AudioProcessorEditor (&p), mainAudio(p)
{
  webBrowser = std::make_unique<BrowserComponent>(
      juce::WebBrowserComponent::Options()
      //.withEventListener("load", [this](const juce::var& event) { onPageLoad(event); })
      .withNativeIntegrationEnabled(true)
    );


  juce::ignoreUnused(mainAudio);

  addAndMakeVisible(*webBrowser);

  // Set the initial size of the editor
  setSize(800, 600);

  // Load the ESPN website
  webBrowser->goToURL("http://0.0.0.0:3000");
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
