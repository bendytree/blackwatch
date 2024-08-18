<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';
import { clamp, roundTo } from '@/utils/math';
import RedOff from "./images/meter-red-off.png";
import RedOn from "./images/meter-red-on.png";
import YellowOff from "./images/meter-yellow-off.png";
import YellowOn from "./images/meter-yellow-on.png";
import GreenOff from "./images/meter-green-off.png";
import GreenOn from "./images/meter-green-on.png";

const props = defineProps<{ args: IMeterArgs }>();

export interface IMeterArgs {
  value: number; // RMS from 0 (silent) to 1, linear
}

const images = {
  red: { off: RedOff, on: RedOn, },
  yellow: { off: YellowOff, on: YellowOn, },
  green: { off: GreenOff, on: GreenOn, },
}

const bulbs = [
  { image: images.red, min: 0.707 },
  { image: images.yellow, min: (0.707 + 0.316) / 2 },
  { image: images.yellow, min: 0.316 },
  { image: images.green, min: (0.316 / 5) * 4 },
  { image: images.green, min: (0.316 / 5) * 3 },
  { image: images.green, min: (0.316 / 5) * 2 },
  { image: images.green, min: .02 },
];

const getBulb = (n:number):string => {
  const b = bulbs[n-1];
  const isOn = props.args.value >= b.min;
  return isOn ? b.image.on : b.image.off;
};

</script>

<template>
  <div class="meter">
    <div v-for="n of 7"
        class="meter-bulb"
        :style="{backgroundImage: `url(${JSON.stringify(getBulb(n))})` }"
    ></div>
  </div>
</template>

<style scoped lang="less">
.meter {
  position: absolute;

  .meter-bulb {
    width: unit((134 / 2), px);
    height: unit((90 / 2), px);
    background-size: 100% 100%;
    overflow: hidden;
  }
}

</style>
