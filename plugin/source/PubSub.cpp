//
// Created by Josh Wright on 3/9/24.
//

#include "PubSub.h"
#include <map>

void PubSub::on(const std::string& event, Callback callback) {
    listeners[event].push_back(callback);
}

void PubSub::trigger(const std::string& event) {
    std::cout << "trigger: " << event << std::endl;

    auto it = listeners.find(event);
    if (it != listeners.end()) {
        for (auto& callback : it->second) {
            callback();
        }
    }
}
