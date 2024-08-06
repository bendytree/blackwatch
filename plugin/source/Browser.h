#pragma once

#include "juce_core/juce_core.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "BinaryData.h"
#include "Types.h"

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
                                          juce::Identifier("getModel"),
                                          [](const juce::Array<juce::var>& args, const juce::WebBrowserComponent::NativeFunctionCompletion& completion) {
                                            juce::var result = juce::var(IGuiModel::current.to_json());
                                            completion(result);
                                          })
                                      .withNativeFunction(
                                          juce::Identifier("setModel"),
                                          [](const juce::Array<juce::var>& args, const juce::WebBrowserComponent::NativeFunctionCompletion& completion) {
                                            juce::String firstArg = args[0].toString();
                                            auto m = IGuiModel::from_json(firstArg);
                                            IGuiModel::current.assign(m);
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
                                      //.withEventListener("load", [this](const juce::var& event) { onPageLoad(event); })
                                      .withNativeIntegrationEnabled(true))
  {
#if JUCE_DEBUG
    this->goToURL("http://localhost:8531");
#else
    this->goToURL(BrowserComponent::getResourceProviderRoot());
#endif
  }
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserComponent)
};
