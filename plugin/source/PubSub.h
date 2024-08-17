

#include <juce_core/juce_core.h>

class PubSubEvent {
public:
  inline static const std::string GuiModelSampleChanged = "GuiModelSampleChanged";
  inline static const std::string GuiModelSettingsChanged = "GuiModelSettingsChanged";
};

class PubSub {
public:
    using Callback = std::function<void()>;

    static PubSub& shared() {
        static PubSub instance;
        return instance;
    }

    void on(const std::string& event, Callback callback);
    void trigger(const std::string& event);

private:
    std::map<std::string, std::vector<Callback>> listeners;
};


