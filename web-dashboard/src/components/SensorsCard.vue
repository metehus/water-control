<script setup lang="ts">
import { useConnectionStore } from '@/stores/connection';
import { storeToRefs } from 'pinia';

const store = useConnectionStore()

const { deviceState } = storeToRefs(store)
</script>

<template>
  <div>
    <div class="data-item">
    <vue-feather type="thermometer"></vue-feather>
    <div class="content">
      <label>
        Temperatura
      </label>
      <span>
        {{ deviceState?.temperature.toFixed(1) }} ℃
      </span>
    </div>
  </div>

  <div class="data-item">
    <vue-feather type="droplet"></vue-feather>
    <div class="content">
      <label>
        Humidade
      </label>
      <span>
        {{ deviceState?.humidity.toFixed(1) }}%
      </span>
    </div>
  </div>
  </div>
</template>

<style scoped>
.data-item {
  display: flex;
  align-items: center;
  gap: 13px;
}

.data-item:not(:last-child) {
  margin-bottom: 10px;
}

.data-item label {
  text-transform: uppercase;
  font-weight: 700;
  color: var(--bluegray-400);
  font-size: 13px;
}

.data-item span {
  font-size: 30px;
}

.content {
  display: flex;
  flex-direction: column;
}
</style>