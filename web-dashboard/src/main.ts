import 'primevue/resources/themes/aura-light-amber/theme.css'
import './assets/main.css'

import { createApp } from 'vue'
import { createPinia } from 'pinia'
import PrimeVue from 'primevue/config'
import VueFeather from 'vue-feather'

import App from './App.vue'
import router from './router'

const app = createApp(App)

app.use(createPinia())
// @ts-ignore
app.component(VueFeather.name, VueFeather)
app.use(PrimeVue)
app.use(router)

app.mount('#app')
