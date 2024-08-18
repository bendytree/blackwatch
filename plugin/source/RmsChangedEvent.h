#pragma once

#include <functional>
#include <vector>
#include <memory>

class RmsChangedEvent
{
public:
  struct Subscription
  {
    std::function<void()> unsubscribe;
  };

  using Callback = std::function<void(float left, float right)>;

  static void trigger(float left, float right);
  static void subscribe(Callback callback);
  static void unsubscribe();

private:
  static Callback callback;
};
