<script setup lang="ts">
import { useConnectionStore } from '@/stores/connection';
import { storeToRefs } from 'pinia';
import { computed } from 'vue';

const store = useConnectionStore()

const { deviceState } = storeToRefs(store)

const grayPercent = computed(() => {
  if (!store.deviceState) return 100 + '%'

  return ((1 - store.deviceState?.waterLevel) * 100) + '%'
})
</script>

<template>
  <div class="">
    <p class="text-sm font-semibold text-center w-full mb-1">
      ({{ deviceState?.waterDistance }} cm)
    </p>
    <svg width="140px" viewBox="0 0 29 38" fill="none" xmlns="http://www.w3.org/2000/svg">
      <rect x="1" y="0" width="27" height="100%" fill="var(--cyan-400)" />
      <rect x="1" y="0" width="27" :height="grayPercent" fill="var(--bluegray-100)" />
      <path d="M29 38V39H30V38H29ZM0 38H-1V39H0V38ZM28 0V38H30V0H28ZM29 37H0V39H29V37ZM1 38V0H-1V38H1Z" fill="black" mask="url(#path-1-inside-1_8_2)" />

      <text
      x="50%"
      y="56%"
      fill="black"
      text-anchor="middle"
      font-size="9px"
      >{{ (deviceState!.waterLevel * 100) }}%</text>
    </svg>

  </div>
</template>