<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue';
import { juce } from '@/gui/juce';
import { autoResizeRootEl } from '@/gui/root-auto-size';
import Knob, { type IKnobArgs } from '@/gui/knob.vue';
import LedDisplay from '@/gui/led-display.vue';
import TheFader, { type IFaderArgs } from '@/gui/fader.vue';
import { clamp, roundTo } from '@/utils/math';

const model = reactive<IGuiModel>({ } as any);
const elRef = ref<HTMLDivElement>(null as any);

onMounted(() => {
  autoResizeRootEl(elRef.value);
});

const timeFaderArgs:Omit<IFaderArgs, 'value'> = {
  valToLabel: s => `${roundTo(s, 3)}s`,
  valToPerc: val => Math.pow(clamp((val - 0.01) / 4.99, 0, 1), 1/2),
  percToVal: p => 0.01 + Math.pow(clamp(p, 0, 1), 2) * 4.99,
};

const attackArgs:IFaderArgs = reactive({
  value: 0.1,
  ...timeFaderArgs,
});

const decayArgs:IFaderArgs = reactive({
  value: 0.1,
  ...timeFaderArgs,
});

const sustainArgs:IFaderArgs = reactive({
  value: 1,
  valToLabel: s => `${roundTo(s * 100, 0)}%`,
  valToPerc: val => clamp(val, 0, 1),
  percToVal: p => clamp(p, 0, 1),
  valToStr: (val:number) => {
    return String(roundTo(val * 100, 0));
  },
  strToVal: (s) => {
    const x = parseFloat(s) / 100;
    return clamp(x, 0, 1);
  },
});

const releaseArgs:IFaderArgs = reactive({
  value: 0.1,
  ...timeFaderArgs,
});

const modDepthArgs:IFaderArgs = reactive({
  value: 0.1,
  valToLabel: s => `${roundTo(s * 100, 0)}%`,
  valToPerc: val => clamp(val, 0, 1),
  percToVal: p => clamp(p, 0, 1),
  valToStr: (val:number) => {
    return String(roundTo(val * 100, 0));
  },
  strToVal: (s) => {
    const x = parseFloat(s) / 100;
    return clamp(x, 0, 1);
  },
});

const modRateArgs:IFaderArgs = reactive({
  value: 1,
  valToLabel: s => `${roundTo(s, 1)} Hz`,
  valToPerc: val => Math.pow(clamp(val / 20, 0, 1), 1/3),
  percToVal: p => 0.1 + Math.pow(clamp(p, 0, 1), 3) * 19.9,
  parse: (s:string) => parseFloat(s),
});

const hpFreqArgs:IKnobArgs = reactive({
  value: 10,
  min: 10,
  max: 20000,
  scale: 'pitch',
  decimals: 0,
});

const hpResonanceArgs:IKnobArgs = reactive({
  value: 1.0 / Math.sqrt(2.0),
  min: 0,
  max: 2,
  scale: 'linear',
  decimals: 2,
});

const lpFreqArgs:IKnobArgs = reactive({
  value: 40000,
  min: 10,
  max: 40000,
  scale: 'pitch',
  decimals: 0,
});

const lpResonanceArgs:IKnobArgs = reactive({
  value: 1.0 / Math.sqrt(2.0),
  min: 0,
  max: 2,
  scale: 'linear',
  decimals: 2,
});

const modulationMixArgs:IKnobArgs = reactive({
  value: 0.5,
  min: 0,
  max: 1,
  scale: 'linear',
  decimals: 2,
});

</script>

<template>
  <div class="root" ref="elRef">
    <led-display :title="'01. CONCERT WAVE'" />
    <knob :style="{ left: `${629/2}px`, top: `${1474/2}px` }" :args="hpFreqArgs" />
    <knob :style="{ left: `${1256/2}px`, top: `${1474/2}px` }" :args="hpResonanceArgs" />
    <knob :style="{ left: `${3214/2}px`, top: `${1474/2}px` }" :args="lpFreqArgs" />
    <knob :style="{ left: `${3845/2}px`, top: `${1474/2}px` }" :args="lpResonanceArgs" />
    <knob :style="{ left: `${3214/2}px`, top: `${568/2}px` }" :args="modulationMixArgs" />
    <the-fader :style="{ left: `${1992/2}px`, top: `728px` }" :args="attackArgs" />
    <the-fader :style="{ left: `${2204/2}px`, top: `728px` }" :args="decayArgs" />
    <the-fader :style="{ left: `${2416/2}px`, top: `728px` }" :args="sustainArgs" />
    <the-fader :style="{ left: `${2628/2}px`, top: `728px` }" :args="releaseArgs" />

    <the-fader :style="{ left: `1870px`, top: `232px` }" :args="modDepthArgs" />
    <the-fader :style="{ left: `1996px`, top: `232px` }" :args="modRateArgs" />
  </div>
</template>

<style lang="less" scoped>
.root {
  background-image: url("gui/images/bg.jpg");
  background-size: 100% 100%;
  width: 2400px;
  height: 1212px;
  transform-origin: 0 0;
}
</style>
