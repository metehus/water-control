<script setup lang="ts">
import {useConnectionStore} from "@/stores/connection";
import Dialog from 'primevue/dialog'
import InputText from 'primevue/inputtext'
import Button from 'primevue/button'
import {computed, ref} from "vue";
import { storeToRefs } from "pinia";

const userValue = ref('');
const passwordValue = ref('');

const store = useConnectionStore()
const { filledUser, connected, connect: mqttConnect } = store

const dialogOpen = computed(() => !filledUser && !connected)

console.log(filledUser, connected)

function connect() {
  console.log(userValue.value)
  console.log(passwordValue.value)
  mqttConnect(userValue.value, passwordValue.value)
}
</script>

<template>
  <Dialog v-model:visible="dialogOpen" modal header="Login">
    <div class="form-item">
      <label for="username">Username</label>
      <InputText v-model="userValue" autocomplete="off" />
    </div>
    <div class="form-item">
      <label for="email">Senha</label>
      <InputText v-model="passwordValue" autocomplete="off" />
    </div>
    <div class="flex justify-content-end gap-2">
      <Button type="button" label="Conectar" severity="primary" @click="connect"></Button>
    </div>
  </Dialog>
</template>

<style scoped>
.form-item {
  display: flex;
  flex-direction: column;
  margin-bottom: 6px;
}
</style>