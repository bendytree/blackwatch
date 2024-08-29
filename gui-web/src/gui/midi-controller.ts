import type { Ref } from 'vue';
import './libs/ctxmenu.js';
import './libs/pubsub.js';
import { clamp } from '@/utils/math';

interface IMidiEvent {
  num: number, val: number, deviceName: string;
}
export interface ISetupMidiControllerArgs {
  el: HTMLDivElement;
  perc: Ref<number>;
}

export const setupMidiController = ({ el, perc }:ISetupMidiControllerArgs) => {
  let firstEvent:IMidiEvent = null;
  const subscriptions = [];
  const connect = () => {
    const s = pubsub.subscribeOnce('midi-changed', (_firstEvent:IMidiEvent) => {
      firstEvent = _firstEvent;
      const s2 = pubsub.subscribe('midi-changed', (e:IMidiEvent) => {
        if (!firstEvent) return;
        if (e.deviceName !== firstEvent.deviceName) return;
        if (e.num !== firstEvent.num) return;
        perc.value = clamp((e.val / 127) || 0, 0, 1);
      });
      subscriptions.push(s2);
    });
    subscriptions.push(s);
  };
  const disconnect = () => {
    firstEvent = null;
    for (const s of subscriptions) {
      pubsub.unsubscribe(s);
    }
  };

  el.addEventListener('contextmenu', (event) => {
    event.preventDefault();
    event.stopPropagation();
    const isConnecting = subscriptions.length > 0;
    ctxmenu.show([
      firstEvent ? { text: `${firstEvent.deviceName} (ch. ${firstEvent.num})` } : null,
      isConnecting ? null : { text: 'Connect to next midi control...', action: connect },
      isConnecting ? { text: 'Disconnect MIDI', action: disconnect } : null,
    ].filter(x => x), event);
  });
};


window.midiChanged = (num: number, val: number, deviceName: string) => {
  const event:IMidiEvent = { num, val, deviceName };
  pubsub.publish('midi-changed', [event]);
};

// setInterval(() => {
//   window.midiChanged(75, Math.round(Math.random() * 127), 'Josh MIDI');
// }, 1000);
