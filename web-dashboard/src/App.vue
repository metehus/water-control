<script setup lang="ts">

import {RouterLink, RouterView} from 'vue-router'
import HelloWorld from './components/HelloWorld.vue'
import AppMenu from "@/components/AppMenu.vue";
</script>

<template>
  <header>
    <AppMenu/>
  </header>

  <RouterView/>
</template>

<style scoped>
body {
  max-width: 1280px;
  margin: 0 auto;
  padding: 0 8px;
}
</style>
