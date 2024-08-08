<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue';

const props = defineProps<{ title: string }>();

const label = computed<string>(() => {
  const t = String(props.title || '');
  if (t.length <= 20) return t;
  return t.substring(0, 19).replace(/\s+$/, '') + '…';
});
</script>

<template>
  <div class="led-display" ref="divRef">
    <div class="led-display-text">{{ label }}</div>
  </div>
  <div class="btn-led-up"></div>
  <div class="btn-led-down"></div>
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
