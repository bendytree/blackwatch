<script setup lang="ts">
import { onMounted, reactive, ref, toRef, watch } from 'vue';
import { autoResizeRootEl } from '@/gui/root-auto-size';
import Knob, { type IKnobArgs } from '@/gui/knob.vue';
import LedDisplay from '@/gui/led-display.vue';
import TheFader, { type IFaderArgs } from '@/gui/fader.vue';
import TheMeter, { type IMeterArgs } from '@/gui/meter.vue';
import { clamp, roundTo } from '@/utils/math';
import _ from 'lodash';
import { juce } from '@/gui/juce';

const props = defineProps<{ settings: IAppSettings }>();
const settings = reactive(props.settings);
const elRef = ref<HTMLDivElement>(null as any);

onMounted(() => {
  autoResizeRootEl(elRef.value);
});

const timeFaderArgs:Omit<IFaderArgs, 'value'> = {
  valToLabel: s => `${roundTo(s, 3)}s`,
  valToPerc: val => Math.pow(clamp((val - 0.01) / 4.99, 0, 1), 1/2),
  percToVal: p => roundTo(0.01 + Math.pow(clamp(p, 0, 1), 2) * 4.99, 4),
};

const attackArgs:IFaderArgs = reactive({
  value: toRef(settings.gui, 'attack'),
  ...timeFaderArgs,
});

const decayArgs:IFaderArgs = reactive({
  value: toRef(settings.gui, 'decay'),
  ...timeFaderArgs,
});

const sustainArgs:IFaderArgs = reactive({
  value: toRef(settings.gui, 'sustain'),
  valToLabel: s => `${roundTo(s * 100, 0)}%`,
  valToPerc: val => clamp(val, 0, 1),
  percToVal: p => roundTo(clamp(p, 0, 1), 4),
  valToStr: (val:number) => {
    return String(roundTo(val * 100, 0));
  },
  strToVal: (s) => {
    const x = parseFloat(s) / 100;
    return clamp(x, 0, 1);
  },
});

const releaseArgs:IFaderArgs = reactive({
  value: toRef(settings.gui, 'release'),
  ...timeFaderArgs,
});

const modDepthArgs:IFaderArgs = reactive({
  value: toRef(settings.gui, 'modDepth'),
  valToLabel: s => `${roundTo(s * 100, 0)}%`,
  valToPerc: val => clamp(val, 0, 1),
  percToVal: p => roundTo(clamp(p, 0, 1), 4),
  valToStr: (val:number) => {
    return String(roundTo(val * 100, 0));
  },
  strToVal: (s) => {
    const x = parseFloat(s) / 100;
    return clamp(x, 0, 1);
  },
});

const modRateArgs:IFaderArgs = reactive({
  value: toRef(settings.gui, 'modRate'),
  valToLabel: s => `${roundTo(s, 1)} Hz`,
  valToPerc: val => Math.pow(clamp(val / 20, 0, 1), 1/3),
  percToVal: p => roundTo(0.1 + Math.pow(clamp(p, 0, 1), 3) * 19.9, 4),
  parse: (s:string) => parseFloat(s),
});

const hpFreqArgs:IKnobArgs = reactive({
  value: toRef(settings.gui, 'hpFreq'),
  min: 10,
  max: 20000,
  scale: 'pitch',
  decimals: 0,
});

const hpResonanceArgs:IKnobArgs = reactive({
  value: toRef(settings.gui, 'hpResonance'),
  min: 0,
  max: 1,
  scale: 'linear',
  decimals: 2,
});

const lpFreqArgs:IKnobArgs = reactive({
  value: toRef(settings.gui, 'lpFreq'),
  min: 10,
  max: 30000,
  scale: 'pitch',
  decimals: 0,
});

const lpResonanceArgs:IKnobArgs = reactive({
  value: toRef(settings.gui, 'lpResonance'),
  min: 0,
  max: 1,
  scale: 'linear',
  decimals: 2,
});

const modulationMixArgs:IKnobArgs = reactive({
  value: toRef(settings.gui, 'modMix'),
  min: 0,
  max: 1,
  scale: 'linear',
  decimals: 2,
});

const lMeterArgs:IMeterArgs = reactive({
  value: 0,
});

const rMeterArgs:IMeterArgs = reactive({
  value: 0,
});

window.updateRmsLevels = (l:number, r:number) => {
  lMeterArgs.value = l;
  rMeterArgs.value = r;
  //console.log(`window.updateRmsLevels`, l, r);
};

// setInterval(() => {
//   rMeterArgs.value = lMeterArgs.value = Math.random();
// }, 100);

const saveNow = async () => {
  await juce.setAppSettings(settings);
};

const saveSoon = _.throttle(saveNow, 20, { trailing: true });

watch(() => settings, saveSoon, { deep: true });

</script>

<template>
  <div class="root" ref="elRef">
    <led-display v-model="settings.gui.sampleId" />
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

    <the-meter :style="{ left: `1130px`, top: `200px` }" :args="lMeterArgs" />
    <the-meter :style="{ left: `1220px`, top: `200px` }" :args="rMeterArgs" />
  </div>
</template>

<style lang="less" scoped>
.root {
  background-image: url("images/bg.jpg");
  background-size: 100% 100%;
  width: 2400px;
  height: 1200px;
  transform-origin: 0 0;
}
</style>
