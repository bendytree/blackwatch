

#include "./MainGui.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "Browser.h"



MainGui::MainGui(MainAudio& p) : juce::AudioProcessorEditor (&p), mainAudio(p)
{
  webBrowser = std::make_unique<BrowserComponent>();

  juce::ignoreUnused(mainAudio);

  addAndMakeVisible(*webBrowser);

  // Set the initial size of the editor
  setSize(1000, 500);


  // Allow the component to be resized
  setResizable(true, true);

  // Set the resize limits to maintain a 2:1 aspect ratio
  setResizeLimits(400, 200, 2000, 1000);

  // Create and set a ComponentBoundsConstrainer to enforce the aspect ratio
  constrainer = std::make_unique<juce::ComponentBoundsConstrainer>();
  constrainer->setFixedAspectRatio(2.0);
  setConstrainer(constrainer.get());
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


