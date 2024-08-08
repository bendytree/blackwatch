<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue';
import { juce } from '@/juce';
import { autoResizeRootEl } from '@/root-auto-size';
import Knob, { type IKnobArgs } from '@/knob.vue';
import LedDisplay from '@/led-display.vue';

const model = reactive<IGuiModel>({ } as any);
const elRef = ref<HTMLDivElement>(null as any);

onMounted(() => {
  autoResizeRootEl(elRef.value);
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
  </div>
</template>

<style lang="less" scoped>
.root {
  background-image: url("./images/bg.jpg");
  background-size: 100% 100%;
  width: 2400px;
  height: 1212px;
  transform-origin: 0 0;
}
</style>
