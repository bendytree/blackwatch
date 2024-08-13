

declare global {
  interface IGuiModel {
    sampleId: string;
    attack: number; // seconds
    decay: number; // seconds
    sustain: number; // 0 through 1
    release: number; // seconds
    hpFreq: number; // Hz
    hpResonance: number; // 0 through 2
    lpFreq: number; // Hz
    lpResonance: number; // 0 through 2
    modMix: number; // 0 through 1
    modDepth: number; // 0 through 1
    modRate: number; // Hz
  }

  interface IAppSettings {
    gui: IGuiModel;
  }
}

export {};
