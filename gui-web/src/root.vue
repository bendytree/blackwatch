<script setup lang="ts">
import { ref } from 'vue';
import { juce } from '@/gui/juce';
import TheGui from './gui/gui.vue';
import { setTimeoutAsync } from '@/utils/timeout';
const settings = ref<IAppSettings | null>(null);
const error = ref<string | null>(null);

const load = async () => {
  try {
    error.value = null;
    settings.value = await juce.getAppSettings();
  }catch (e){
    await setTimeoutAsync(2000);
    error.value = String(e);
  }
};

load();
</script>

<template>
  <the-gui v-if="settings" :settings="settings" />
  <div class="root" v-else>
    <div v-if="error">
      <div style="margin-bottom: 10px;">{{ error }}</div>
      <button @click="load" type="button">RETRY</button>
    </div>
    <div v-else>LOADING...</div>
  </div>
</template>

<style scoped lang="less">
.root {
  position: fixed;
  inset: 0 0 0 0;
  display: flex;
  align-items: center;
  justify-content: center;
  color:#aaa;
  text-align: center;
  font-size: 3vw;
}
</style>
