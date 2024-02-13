<script setup lang="ts">
import Card from "primevue/card";
import ProgressSpinner from "primevue/progressspinner";
import { useConnectionStore } from "@/stores/connection";
import ConnectionDialog from "@/components/ConnectionDialog.vue";
import SensorsCard from "@/components/SensorsCard.vue";
import WaterLevelVue from "@/components/WaterLevel.vue";
import ButtonsCard from "@/components/ButtonsCard.vue";

const store = useConnectionStore()
</script>

<template>
  <div class="mt-3 mx-4">
    <div v-if="store.connected && store.deviceState !== null" class="w-full">
      <Card class="card">
        <template #content>
          <div class="flex justify-between items-center">
            <SensorsCard />
            <WaterLevelVue />
          </div>
        </template>
      </Card>

      <Card class="card mt-4">
        <template #content>
          <div class="flex justify-between items-center">
            <ButtonsCard />
          </div>
        </template>
      </Card>
    </div>
    <div v-else class="spinner-card w-full">
      <Card class="card">
        <template #content>

          <div class="spinner-card">
            <ProgressSpinner class="spinner" />
            <ConnectionDialog />
          </div>
        </template>
      </Card>

    </div>
  </div>
</template>

<style>
.card {
  border: 1px solid #ddd;
  width: 100%;
}

.spinner-card {
  display: flex;
  width: 100%;
  align-items: center;
}

.spinner {
  width: 40px;
  height: auto;
}

.spinner circle {
  stroke: black;
  animation: p-progress-spinner-dash 1.5s ease-in-out infinite
}
</style>
