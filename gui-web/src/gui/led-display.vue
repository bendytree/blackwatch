<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';
import { showPicker } from '@/picker';
import repo from '../../../repo.json';

const model = defineModel<string>();

const sample = computed(() => {
  return repo.samples.find(s => s.id.toString() === model.value);
});

const label = computed<string>(() => {
  const t = String(sample.value?.title || '');
  if (t.length <= 19) return t;
  return t.substring(0, 18).replace(/\s+$/, '') + '…';
});

const clickDisplay = async () => {
  const results = await showPicker({
    initialValue: model.value,
    title: 'Choose One',
    options: repo.samples.map(s => ({ value: s.id.toString(), label: s.title })),
  });

  if (results.value) {
    model.value = results.value;
  }
};

const move = async (dir:number) => {
  let idx = repo.samples.indexOf(sample.value as any);
  idx = ((idx + dir) + repo.samples.length) % repo.samples.length;
  model.value = repo.samples[idx].id.toString();
};
</script>

<template>
  <div class="led-display" ref="divRef" @click="clickDisplay">
    <div class="led-display-text">{{ label }}</div>
  </div>
  <div class="btn-led-up" @click="move(1)"></div>
  <div class="btn-led-down" @click="move(-1)"></div>
</template>

<style scoped lang="less">
.led-display {
  position: absolute;
  left: 250px;
  top: 402px;
  width: 550px;
  height: 127px;
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  text-align: center;
  cursor: pointer;
  transition: background-color 200ms ease-in-out;

  //box-shadow: 0 0 10px 10px #fff;

  .led-display-text {
    user-select: none;
    pointer-events: none;
    color: #4DFD00;
    font-size:48px;
    font-family: 'DigitalCounter7', monospace;
    text-shadow: 2px 2px 10px fade(#4DFD00, 10%),
      2px -2px 10px fade(#4DFD00, 10%),
      -2px 2px 10px fade(#4DFD00, 10%),
      -2px -2px 10px fade(#4DFD00, 10%);
  }

  &:hover {
    background-color: rgba(0, 255, 0, 5%);
  }
}

.btn-led-up, .btn-led-down {
  position: absolute;
  left: 835px;
  width: 85px;
  height: 62px;
  transition: background-color 200ms ease-in-out;
  cursor: pointer;
  &:hover {
    background-color: rgba(255, 255, 255, 0.1);
  }
}
.btn-led-up {
  top: 402px;
}
.btn-led-down {
  top: 466px;
}

</style>
