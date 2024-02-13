import { defineStore } from "pinia";
import { ref } from "vue";
import mqtt from "mqtt";
import { nanoid } from "nanoid";

interface RawState {
  temp: string
  hum: string
  wLevel: string
  wDistance: string
  wEptDist: string
  wFullDist: string
  pSchActive: string
  pSchMinInterval: string
  pScheduleMaxOn: string
  pActiveLvl: string
  pActive: string
  lastPumpActiveTime: string
  button0: string
  button1: string
  button2: string
}

export interface StateData {
  temperature: number
  humidity: number
  waterLevel: number
  waterDistance: number
  waterEmptyDist: number
  waterFullDist: number
  pumpScheduleActive: boolean
  pumpScheduleMinInterval: number
  pumpScheduleMaxOn: number
  pumpActiveLevel: number
  pumpActive: boolean
  lastPumpActiveTime: number
  button0: boolean
  button1: boolean
  button2: boolean
}

const savedUser = localStorage.getItem("mqttUser");
const savedPass = localStorage.getItem("mqttPass");

export const useConnectionStore = defineStore("connectionStore", () => {
  const connected = ref(false);
  const deviceState = ref<StateData | null>(null)
  const filledUser = ref(savedUser && savedPass);


  const client = mqtt.connect(import.meta.env.VITE_MQTT_URL, {
    username: savedUser || "",
    password: savedPass || "",
    clientId: `web-dashboard-${nanoid()}`,
  });
  client.subscribe('weatherLite2/state')

  function messageHandler(topic: string, payload: Buffer) {
    if (topic === 'weatherLite2/state') {
      const data = new URLSearchParams(payload.toString())
      const rawData = Object.fromEntries(data.entries()) as unknown as RawState
      deviceState.value = {
        temperature: parseFloat(rawData.temp),
        humidity: parseFloat(rawData.hum),
        waterLevel: parseFloat(rawData.wLevel),
        waterDistance: parseInt(rawData.wDistance),
        waterEmptyDist: parseInt(rawData.wEptDist),
        waterFullDist: parseInt(rawData.wFullDist),
        pumpScheduleActive: rawData.pSchActive === 'true',
        pumpScheduleMinInterval: parseInt(rawData.pSchMinInterval),
        pumpScheduleMaxOn: parseInt(rawData.pScheduleMaxOn),
        pumpActiveLevel: parseFloat(rawData.pActiveLvl),
        pumpActive: rawData.pActive === 'true',
        lastPumpActiveTime: parseInt(rawData.lastPumpActiveTime),
        button0: rawData.button0 === 'true',
        button1: rawData.button1 === 'true',
        button2: rawData.button2 === 'true',
      }
      // @ts-ignore
      window._deviceState = deviceState.value
    }
  }

  client.on('message', messageHandler)
  
  client.on('connect', () => {
    console.log('[MQTT] Connected!')
    connected.value = true
  })

  client.on('error', (err) => {
    console.log('[MQTT] Error:')
    console.error(err)
  })

  client.on('disconnect', () => {
    connected.value = false
  })

  function connect(user: string, pass: string) {
    client.options.username = user
    client.options.password = pass
    client.connect()
    localStorage.setItem("mqttUser", user);
    localStorage.setItem("mqttPass", pass);
  }

  return { client, connected, deviceState, connect, filledUser };
});
