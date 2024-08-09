import { createApp } from 'vue';
import type { IPickerParams, IPickerResults } from '@/picker/picker.vue';
import Picker from './picker.vue';

export const showPicker = (params: IPickerParams):Promise<IPickerResults> => {
  return new Promise<IPickerResults>(resolve => {
    const container = document.createElement('div');
    document.body.appendChild(container);

    const app = createApp(Picker, {
      params,
      done (results:IPickerResults) {
        app.unmount();
        document.body.removeChild(container);
        resolve(results);
      },
    });
    app.component('picker', Picker);
    app.mount(container);
  });
};
