<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';
import { clamp, roundTo } from '@/utils/math';

const props = defineProps<{ args: IKnobArgs }>();
const divRef = ref<HTMLDivElement | null>(null);

const { scale, min, max } = props.args;
const scalers = [
  {
    type: 'linear' as const,
    percToVal: (p:number) => min + p * (max - min),
    valToPerc: (val:number) => (val - min) / (max - min),
    suffix: '',
  },
  // log2
  {
    type: 'pitch' as const,
    percToVal: (p:number) => effectiveMin * Math.pow(2, p * Math.log2(max / effectiveMin)),
    valToPerc: (val:number) => Math.log2(val / effectiveMin) / Math.log2(max / effectiveMin),
    suffix: 'Hz',
  },
  // log10
  {
    type: 'decibels' as const,
    percToVal: (p:number) => effectiveMin * Math.pow(10, p * (Math.log10(max / effectiveMin))),
    valToPerc: (val:number) => Math.log10(val / effectiveMin) / Math.log10(max / effectiveMin),
    suffix: 'dB',
  },
];

export declare type IKnobScaleType = typeof scalers[number]['type'];

export interface IKnobArgs {
  value: number;
  min: number;
  max: number;
  scale: IKnobScaleType;
  decimals: number;
}

const effectiveMin = Math.max(min, 10);
const scaler = scalers.find(s => s.type === props.args.scale);
const percToVal = (p: number): number => {
  const v = scaler.percToVal(p);
  return roundTo(clamp(v, min, max), props.args.decimals || 0);
};

const valToPerc = (val: number): number => {
  const p = scaler.valToPerc(val);
  return clamp(p, 0, 1);
};

const active = ref<boolean>(false);
const perc = ref<number>(valToPerc(props.args.value));
const rotation = computed(() => perc.value * 278);
const label = computed(() => {
  let x = props.args.value;
  const power = (() => {
    if (x >= 1000000000) {
      x /= 1000000000;
      return 'G';
    } else if (x >= 1000000) {
      x /= 1000000;
      return 'M';
    } else if (x >= 1000) {
      x /= 1000;
      return 'K';
    }
  })();
  const suffix = [power, scaler.suffix].filter(x => x).join('');
  return `${roundTo(x, 1)}${suffix ? ` ${suffix}` : ``}`;
});

watch(() => perc.value, (p: number) => {
  props.args.value = percToVal(p);
});

let startY = 0;
let startVal = 0;

onMounted(() => {
  divRef.value.addEventListener('mousedown', (event) => {
    active.value = true;
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
  active.value = false;
}

function onDoubleClick() {
  const newValStr = prompt(scaler.suffix, String(props.args.value ?? ''));
  const newVal = parseFloat(newValStr || '');
  if (isNaN(newVal)) return;
  perc.value = valToPerc(newVal);
}
</script>

<template>
  <div class="knob" ref="divRef" :class="{ active }" @dblclick="onDoubleClick">
    <div class="knob-marker" :style="{ transform: `rotate(${rotation}deg)` }"></div>
    <div class="knob-label">{{ label }}</div>
  </div>
</template>

<style scoped lang="less">
.knob {
  width: unit((351 / 2), px);
  height: unit((351 / 2), px);
  position: absolute;
  //box-shadow: 0 0 5px yellow;
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;

  .knob-marker {
    width: 100%;
    height: 100%;
    background-image: url('./images/knob-marker-off.png');
    position: absolute;
    background-size: 100% 100%;
    pointer-events: none;
  }

  .knob-label {
    font-size:30px;
    font-weight: bold;
    color: #999;
    user-select: none;
    pointer-events: none;
    transition: color 200ms ease-in-out;
    text-shadow: 2px 2px 3px rgba(0,0,0,0.5), -2px -2px 3px rgba(0,0,0,0.5), 2px -2px 3px rgba(0,0,0,0.5), -2px 2px 3px rgba(0,0,0,0.5);
  }

  &.active {
    .knob-marker {
      background-image: url('./images/knob-marker-on.png');
    }
    .knob-label {
      color: #fff;
    }
  }
}

</style>
