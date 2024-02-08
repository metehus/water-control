<script setup lang="ts">

import Menubar from "primevue/menubar";
import { ref } from "vue";

const items = ref([
  {
    label: 'Início',
    icon: 'home'
  },
  {
    label: 'Configurações',
    icon: 'settings'
  }
]);

</script>

<template>
  <nav>
    <Menubar :model="items">
      <template #itemicon="{ item }">
        <vue-feather class="menu-icon" :type="item.icon"></vue-feather>
      </template>
    </Menubar>
  </nav>
</template>

<style scoped>
.menu-icon {
  margin-right: 8px;
}
nav {
  max-width: 1280px;
  margin-left: auto;
  margin-right: auto;
}
</style>