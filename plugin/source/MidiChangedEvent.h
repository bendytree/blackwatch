#pragma once

#include <functional>
#include <vector>
#include <memory>

class MidiChangedEvent
{
public:
  struct Subscription
  {
    std::function<void()> unsubscribe;
  };

  using Callback = std::function<void(int num, int val)>;

  static void trigger(int num, int val);
  static void subscribe(Callback callback);
  static void unsubscribe();

private:
  static Callback callback;
};
