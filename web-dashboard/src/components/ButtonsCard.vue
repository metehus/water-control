<script setup lang="ts">
import ButtonItem from './ButtonItem.vue';
</script>

<template>
  <div class="flex justify-evenly w-full">
    <ButtonItem title="Bomba mina" :button-index="0" />
    <ButtonItem title="Bomba 1" :button-index="1" />
    <ButtonItem title="Bomba 2" :button-index="2" />
  </div>
</template>