<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';
import { clamp, roundTo } from '@/utils/math';

const props = defineProps<{ args: IKnobArgs }>();
const divRef = ref<HTMLDivElement | null>(null);

export declare type IKnobScaleType = 'linear'
    | 'pitch' // log2
    | 'decibels'; // log10

export interface IKnobArgs {
  value: number;
  min: number;
  max: number;
  scale: IKnobScaleType;
  decimals: number;
}

const { scale, min, max } = props.args;
const effectiveMin = Math.max(min, 20);
const percToVal = (p: number): number => {
  const v = (() => {
    if (scale === 'linear') return min + p * (max - min);
    if (scale === 'pitch') return effectiveMin * Math.pow(2, p * Math.log2(max / effectiveMin));
    if (scale === 'decibels') return effectiveMin * Math.pow(10, p * (Math.log10(max / effectiveMin)));
    return min;
  })();
  return roundTo(clamp(v, min, max), props.args.decimals || 0);
};

const valToPerc = (val: number): number => {
  const p = (() => {
    if (scale === 'linear') return (val - min) / (max - min);
    if (scale === 'pitch') return Math.log2(val / effectiveMin) / Math.log2(max / effectiveMin);
    if (scale === 'decibels') return Math.log10(val / effectiveMin) / Math.log10(max / effectiveMin);
    return 0;
  })();
  return clamp(p, 0, 1);
};

const active = ref<boolean>(false);
let toActive:number = 0;
const perc = ref<number>(valToPerc(props.args.value));

const rotation = computed(() => perc.value * 278);

watch(() => perc.value, (p: number) => {
  props.args.value = percToVal(p);
  active.value = true;
  clearTimeout(toActive);
  toActive = setTimeout(() => active.value = false, 1000);
})

let startY = 0;
let startVal = 0;

onMounted(() => {
  divRef.value.addEventListener('mousedown', (event) => {
    startY = event.clientY;
    startVal = perc.value;
    document.addEventListener('mousemove', onMouseMove);
    document.addEventListener('mouseup', onMouseUp);
  })
});

function onMouseMove(event:MouseEvent) {
  const distanceY = startY - event.clientY;
  const amount = distanceY / 150;
  const newVal = Math.max(Math.min(startVal + amount, 1), 0);
  perc.value = newVal;
}

function onMouseUp() {
  document.removeEventListener('mousemove', onMouseMove);
  document.removeEventListener('mouseup', onMouseUp);
}

function onDoubleClick() {
  const newValStr = prompt(`Hz`, String(props.args.value ?? ''));
  const newVal = parseFloat(newValStr || '');
  if (isNaN(newVal)) return;
  perc.value = valToPerc(newVal);
}

</script>

<template>
  <div class="knob" ref="divRef" :class="{ active }" @dblclick="onDoubleClick">
    <div class="knob-marker" :style="{ transform: `rotate(${rotation}deg)` }"></div>
    <div class="knob-label">{{ props.args.value.toLocaleString() }} Hz</div>
  </div>
</template>

<style scoped lang="less">
.knob {
  width: unit((351 / 2), px);
  height: unit((351 / 2), px);
  position: relative;
  //box-shadow: 0 0 5px yellow;
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;

  .knob-marker {
    width: 100%;
    height: 100%;
    background-image: url('./images/knob-marker.png');
    position: absolute;
    background-size: 100% 100%;
    pointer-events: none;
  }

  .knob-label {
    font-size:32px;
    color: #aaa;
    user-select: none;
    pointer-events: none;
    transition: color 200ms ease-in-out;
  }

  &.active {
    .knob-label {
      color: #fff;
    }
  }
}

</style>
