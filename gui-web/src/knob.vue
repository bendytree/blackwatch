<script setup lang="ts">

export interface IKnobArgs {

}

const props = defineProps<{ args: IKnobArgs }>();

</script>

<template>
  <div class="knob">

  </div>
</template>

<style scoped lang="less">
.knob {
  width: unit((351 / 2), px);
  height: unit((351 / 2), px);
  background-image: url('./images/knob-marker.png');
  position: relative;
  background-size: 100% 100%;
  //box-shadow: 0 0 5px yellow;
}

</style>
