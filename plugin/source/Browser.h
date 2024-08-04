#pragma once

#include "juce_core/juce_core.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "Server.h"

class BrowserComponent final : public juce::WebBrowserComponent
{
public:
  explicit BrowserComponent(const juce::WebBrowserComponent::Options& options)
      : juce::WebBrowserComponent(options)
  {
    Server& server = Server::getInstance("0.0.0.0", 3000);
    server.start();
  }
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserComponent)
};
