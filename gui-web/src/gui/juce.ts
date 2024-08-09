
import { getNativeFunction } from '../../../libs/juce/modules/juce_gui_extra/native/javascript/index.js';

export const juce = {
  async getModel ():Promise<IGuiModel> {
    const json = await getNativeFunction("getModel")();
    return JSON.parse(json);
  },
  async setModel (model:IGuiModel):Promise<void> {
    return getNativeFunction("setModel")(JSON.stringify(model));
  },
};
