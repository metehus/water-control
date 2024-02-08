<script setup lang="ts">
import Card from "primevue/card";
import ProgressSpinner from "primevue/ProgressSpinner";
import {useConnectionStore} from "@/stores/connection";
import ConnectionDialog from "@/components/ConnectionDialog.vue";

const store = useConnectionStore()
</script>

<template>
  <Card class="card">
    <template #content>
      <div v-if="store.connected" >
        conectado!

        <p class="m-0">
          Lorem ipsum dolor sit amet, consectetur adipisicing elit. Inventore sed consequuntur error repudiandae numquam deserunt quisquam repellat libero asperiores earum nam nobis, culpa ratione quam perferendis esse, cupiditate neque
          quas!
        </p>
      </div>
      <div v-else class="spinner-card">
        <ProgressSpinner class="spinner" />
        <ConnectionDialog />
      </div>
    </template>
  </Card>
</template>

<style>
.card {
  margin-top: 20px;
  border: 1px solid #ddd;
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
