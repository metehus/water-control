import { defineStore } from "pinia";
import { ref } from "vue";
import mqtt from "mqtt";
import { nanoid } from "nanoid";

const savedUser = localStorage.getItem("mqttUser");
const savedPass = localStorage.getItem("mqttPass");

export const useConnectionStore = defineStore("connectionStore", () => {
  const client = mqtt.connect(import.meta.env.VITE_MQTT_URL, {
    username: savedUser || "",
    password: savedPass || "",
    clientId: `web-dashboard-${nanoid()}`,
  });
  client.subscribe('weatherLite2/state')
  console.log(client.options)

  client.on('connect', () => {
    console.log('[MQTT] Connected!')
  })

  client.on('message', (topic, payload, packet) => {
    console.log(topic)
    console.log(payload.toString())
  })

  client.on('error', (err) => {
    console.log('[MQTT] Error:')
    console.error(err)
  })

  const connected = ref(false);

  const filledUser = ref(savedUser && savedPass);

  function connect(user: string, pass: string) {
    client.options.username = user
    client.options.password = pass
    client.connect()
    localStorage.setItem("mqttUser", user);
    localStorage.setItem("mqttPass", pass);
  }

  return { client, connected, connect, filledUser };
});
