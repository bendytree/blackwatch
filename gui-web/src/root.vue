<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue';
import { juce } from '@/juce';
import { autoResizeRootEl } from '@/root-auto-size';
import Knob, { type IKnobArgs } from '@/knob.vue';

const model = reactive<IGuiModel>({ } as any);
const elRef = ref<HTMLDivElement>(null as any);

onMounted(() => {
  autoResizeRootEl(elRef.value);
});

const hpFreqArgs:IKnobArgs = {

};


</script>

<template>
  <div class="root" ref="elRef">
    <knob :style="{ left: `${629/2}px`, top: `${1474/2}px` }" :args="hpFreqArgs" />
  </div>
</template>

<style src="./reset.less" />

<style lang="less" scoped>
.root {
  background-image: url("./images/bg.jpg");
  background-size: 100% 100%;
  width: 2400px;
  height: 1212px;
  transform-origin: 0 0;
}
</style>
