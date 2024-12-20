#pragma once

#include "juce_core/juce_core.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "IGuiModel.h"
#include "BinaryData.h"
#include "AppSettings.h"
#include "RmsChangedEvent.h"
#include "MidiChangedEvent.h"
#include "BwLogger.h"

// text/html, text/plain, text/javascript
static juce::WebBrowserComponent::Resource createResource(const juce::String& data, const juce::String& mimeType)
{
  std::vector<std::byte> byteData(reinterpret_cast<const std::byte*>(data.toRawUTF8()), reinterpret_cast<const std::byte*>(data.toRawUTF8() + data.getNumBytesAsUTF8()));
  return { byteData, mimeType };
}

class BrowserComponent final : public juce::WebBrowserComponent
{
public:
  explicit BrowserComponent()
      : juce::WebBrowserComponent(juce::WebBrowserComponent::Options()
                                      .withNativeFunction(
                                          juce::Identifier("getAppSettings"),
                                          [](const juce::Array<juce::var>& args, const juce::WebBrowserComponent::NativeFunctionCompletion& completion) {
                                            juce::var result = juce::var(IAppSettings::getCurrent().to_json());
                                            completion(result);
                                          })
                                      .withNativeFunction(
                                          juce::Identifier("setAppSettings"),
                                          [](const juce::Array<juce::var>& args, const juce::WebBrowserComponent::NativeFunctionCompletion& completion) {
                                            juce::String json = args[0].toString();
                                            auto s = IAppSettings::from_json(json);
                                            IAppSettings::assignCurrent(s);
                                            completion(juce::var("ok"));
                                          })
                                      .withResourceProvider([](const juce::String& path) -> std::optional<juce::WebBrowserComponent::Resource>
                                                            {
                                                              if (path == "/") {
                                                                return createResource(juce::String(juce::CharPointer_UTF8(BinaryData::index_html), BinaryData::index_htmlSize), "text/html");
                                                              }else{
                                                                return createResource("Not Found", "text/plain");
                                                              }
                                                            })
                                      .withNativeIntegrationEnabled(true))
  {
//#if JUCE_DEBUG
//    this->goToURL("http://localhost:8531");
//#else
    this->goToURL(BrowserComponent::getResourceProviderRoot());
//#endif

    RmsChangedEvent::subscribe([this](float left, float right) {
      this->evaluateJavascript("window.updateRmsLevels(" + std::to_string(left) + ", " + std::to_string(right) + ");");
    });

    BwLogger::log("Browser - midiSubscribe...");
    MidiChangedEvent::subscribe([this](int num, int val, std::string deviceName) {
      BwLogger::log("Browser - midiSubscribe.callback" + std::to_string(num) + ", " + std::to_string(val));
      this->evaluateJavascript("window.midiChanged(" + std::to_string(num) + ", " + std::to_string(val) + ", `" + deviceName + "`);");
    });
  }

    ~BrowserComponent() override {
      RmsChangedEvent::unsubscribe();
    }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserComponent)

};
