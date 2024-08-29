#include "MidiChangedEvent.h"
#include <algorithm>
#include <utility>

MidiChangedEvent::Callback MidiChangedEvent::callback;

void MidiChangedEvent::trigger(int num, int val, std::string deviceName)
{
  if (MidiChangedEvent::callback)
  {
    MidiChangedEvent::callback(num, val, std::move(deviceName));
  }
}

void MidiChangedEvent::unsubscribe() {
  MidiChangedEvent::callback = nullptr;
}

void MidiChangedEvent::subscribe(Callback callback)
{
  MidiChangedEvent::callback = std::move(callback);
}
