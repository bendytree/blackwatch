#include "MidiChangedEvent.h"
#include <algorithm>
#include <utility>

MidiChangedEvent::Callback MidiChangedEvent::callback;

void MidiChangedEvent::trigger(int num, int val)
{
  if (MidiChangedEvent::callback)
  {
    MidiChangedEvent::callback(num, val);
  }
}

void MidiChangedEvent::unsubscribe() {
  MidiChangedEvent::callback = nullptr;
}

void MidiChangedEvent::subscribe(Callback callback)
{
  MidiChangedEvent::callback = std::move(callback);
}
