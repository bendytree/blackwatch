

<script setup lang="ts">

import { ref } from 'vue';

export interface IPickerOption {
  label: string;
  value: string;
}

export interface IPickerResults {
  value: string;
}

export interface IPickerParams {
  initialValue: string;
  title: string;
  options: IPickerOption[];
}

const props = defineProps<{
  params: IPickerParams,
  done: (results:IPickerResults) => void;
}>();

const value = ref<string>(props.params.initialValue);

const isActive = (option:IPickerOption):boolean => {
  return value.value === option.value;
};

const dismiss = () => {
  document.removeEventListener('keydown', handleEscapeKey);
  props.done({
    value: value.value,
  });
};

const clickClose = () => {
  dismiss();
};

const clickOption = (option:IPickerOption) => {
  value.value = option.value;
  dismiss();
};

document.addEventListener('keydown', handleEscapeKey);

function handleEscapeKey(event: KeyboardEvent) {
  if (event.key === 'Escape') {
    clickClose();
  }
}

</script>

<template>
  <div class="picker-modal" @click="clickClose">
    <div class="picker-modal-body" @click.stop.prevent>
      <div class="btn-close-picker" @click="clickClose">x</div>
      <div class="picker-options">
        <div class="picker-option"
            :class="{ active: isActive(option) }"
            @click="clickOption(option)"
            v-for="option of props.params.options">
          {{ option.label }}
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.picker-modal {
  position: fixed;
  inset: 0 0 0 0;
  background-color: rgba(0, 0, 0, 0.8);
  display: flex;
  flex-direction: row;
  align-items: start;
  justify-content: center;
  overflow-y: scroll;

  .picker-modal-body {
    background-color: #444;
    border-radius: 25px;
    padding: 20px;
    margin:50px 25px;
    flex-grow: 1;

    .btn-close-picker {
      position: absolute;
      top: 5px;
      right: 5px;
      width: 40px;
      height: 40px;
      background-color: rgba(255, 255, 255, 0.7);
      display: flex;
      align-items: center;
      justify-content: center;
      border-radius: 50%;
      font-size: 29px;
      cursor: pointer;

      &:hover {
        background-color: rgba(255, 255, 255, 1);
      }
    }

    .picker-options {
      column-count: auto;
      column-width: 240px;
      column-gap: 20px;
      column-fill: balance;

      .picker-option {
        color: #fff;
        background-color: #555;
        border-radius: 4px;
        margin-bottom: 2px;
        padding: 5px 10px 3px;
        cursor: pointer;

        &:hover {
          background-color: #ddd;
          color: #000;
        }

        &.active {
          background-color: #09c;
          color: #fff;

          &:hover {
            background-color: #0cf;
          }
        }
      }
    }
  }
}
</style>

