
import { getNativeFunction } from '../../../libs/juce/modules/juce_gui_extra/native/javascript/index.js';

const verifyJuce = () => {
  const ok = window.__JUCE__?.initialisationData?.__juce__functions?.includes?.('getAppSettings');
  if (ok) return;
  throw new Error(`JUCE native app is not available.`);
};

export const juce = {
  async getAppSettings ():Promise<IAppSettings> {
    verifyJuce();
    const json = await getNativeFunction("getAppSettings")();
    console.log(`getAppSettings.json`, json);
    return JSON.parse(json);
  },
  async setAppSettings (s:IAppSettings):Promise<void> {
    verifyJuce();
    console.log(`setAppSettings.json`, s);
    return getNativeFunction("setAppSettings")(JSON.stringify(s));
  },
};
