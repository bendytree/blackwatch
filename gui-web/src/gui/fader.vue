<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';
import { clamp, roundTo } from '@/utils/math';

const props = defineProps<{ args: IFaderArgs }>();
const divRef = ref<HTMLDivElement | null>(null);


export interface IFaderArgs {
  value: number;
  percToVal: (val:number) => number;
  valToPerc: (val:number) => number;
  valToLabel: (val:number) => string;
  valToStr?: (val:number) => string;
  strToVal?: (str:string) => number;
}

const { percToVal } = props.args;

const valToPerc = (val: number): number => {
  const p = props.args.valToPerc(val);
  return clamp(p, 0, 1);
};

const active = ref<boolean>(false);
const perc = ref<number>(valToPerc(props.args.value));
const label = computed(() => {
  return props.args.valToLabel(props.args.value);
});

const gripStyle = computed(() => {
  return {
    transform: `translateY(${perc.value * -220}px)`,
  };
});

watch(() => perc.value, (p: number) => {
  props.args.value = roundTo(percToVal(p), 5);
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
  const amount = (distanceY) / (218/2);
  perc.value = Math.max(Math.min(startVal + amount, 1), 0);
}

function onMouseUp() {
  document.removeEventListener('mousemove', onMouseMove);
  document.removeEventListener('mouseup', onMouseUp);
  active.value = false;
}

function onDoubleClick() {
  const oldStr = props.args.valToStr?.(props.args.value) ?? props.args.value ?? '';
  const newValStr = prompt('', String(oldStr));
  if (!newValStr) return;
  const newVal = props.args.strToVal ? props.args.strToVal(newValStr) : parseFloat(newValStr || '');
  if (isNaN(newVal)) return;
  perc.value = valToPerc(newVal);
}
</script>

<template>
  <div class="fader" ref="divRef" :class="{ active }" @dblclick="onDoubleClick">
    <div class="fader-grip" :style="gripStyle">
      <div class="fader-highlight"></div>
    </div>
    <div class="fader-label">{{ label }}</div>
  </div>
</template>

<style scoped lang="less">
.fader {
  width: unit((188 / 2), px);
  height: 310px;
  position: absolute;
  // box-shadow: 0 0 5px yellow;
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  cursor: pointer;

  .fader-grip {
    position: absolute;
    width: 100%;
    height: unit((134 / 2), px);
    bottom: 32px;
    background-image: url('./images/fader.png');
    background-size: 100% 100%;
  }

  .fader-highlight {
    position: absolute;
    top: 30px;
    height: 7px;
    right: 20px;
    left: 20px;
    transition: background-color 200ms ease-in-out;
  }

  .fader-label {
    position: absolute;
    left:-10px;
    right:-10px;
    bottom: 5px;
    color: #aaa;
    font-size:28px;
    transition: color 200ms ease-in-out;
    text-align: center;
  }

  &:hover {
    .fader-highlight {
      background-color: fade(#09f, 90%);
    }
    .fader-label {
      color: #fff;
    }
  }
}

</style>
