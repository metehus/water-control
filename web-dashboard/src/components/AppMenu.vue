<script setup lang="ts">

import Button from "primevue/button";
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
    <!-- <Button icon="home" class="">
      Início

      <template #icon>
        <vue-feather class="menu-icon" :type="class"></vue-feather>
      </template>
    </Button> -->
    <Menubar :model="items" breakpoint="20px" >
      <template #itemicon="{ item }">
        <vue-feather class="menu-icon" :type="item.icon" />
      </template>
    </Menubar>
  </nav>
</template>

<style scoped>
.menu-icon {
  margin-right: 8px;
}
</style>