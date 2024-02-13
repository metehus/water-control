<script setup lang="ts">
import { useConnectionStore } from '@/stores/connection';
import ProgressSpinner from 'primevue/ProgressSpinner';
import InputSwitch from 'primevue/inputswitch';
import { computed, ref, watch, watchEffect } from 'vue';

const props = defineProps<{
  title: string,
  buttonIndex: 0 | 1 | 2
}>()

const loading = ref(false)

const store = useConnectionStore()

const buttonValue = computed(
  () => store.deviceState?.[`button${props.buttonIndex}`]
)

watch(() => buttonValue.value, (value, prevValue) => {
  loading.value = false
})

function toggle() {
  if (loading.value) return
  const newValue = !buttonValue.value
  loading.value = true
  const newValueString = newValue ? 't' : 'f'
  const payload = `${props.buttonIndex}=${newValueString}`
  store.client.publish('weatherLite2/setButtonValue', payload)
}
</script>

<template>
  <div @click="toggle" class="cursor-pointer w-1/6 rounded-lg overflow-hidden flex flex-col items-center gap-3 pb-3" style="background: var(--bluegray-50);">
    <div class="flex flex-col items-center w-full py-4 rounded-lg relative" :style="`color: black; background: var(--${buttonValue ? 'green' : 'red'}-600)`">
      <span>{{ props.title }}</span>
      {{ buttonValue ? 'ON' : 'OFF' }}
      <div v-if="loading" class="button-spinner rounded-lg overflow-hidden absolute">
        <ProgressSpinner class="spinner absolute" />
      </div>
    </div>
    <InputSwitch :model-value="buttonValue" />
  </div>
</template>

<style scoped>
.button-spinner {
  top: 0;
  left: 0;
  backdrop-filter: blur(5px) brightness(0.8);
  width: 100%;
  height: 100%;
}

.button-spinner .spinner {
  left: 50%;
  top: 50%;
  transform: translate(-50%, -50%);
}
</style>