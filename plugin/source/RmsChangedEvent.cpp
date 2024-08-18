#include "RmsChangedEvent.h"
#include <algorithm>
#include <utility>

RmsChangedEvent::Callback RmsChangedEvent::callback;

void RmsChangedEvent::trigger(float left, float right)
{
  if (RmsChangedEvent::callback)
  {
    RmsChangedEvent::callback(left, right);
  }
}

void RmsChangedEvent::unsubscribe() {
  RmsChangedEvent::callback = nullptr;
}

void RmsChangedEvent::subscribe(Callback callback)
{
  RmsChangedEvent::callback = std::move(callback);
}
