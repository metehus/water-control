//
// Created by methi on 25/01/2024.
//

#include "network.h"
#include "network_html.h"
#include "env.h"
#include <WiFi.h>

WiFiClient wifiClient;
bool setupDone = false;

Network::Network(DisplayState &i_displayState) : server(80), mqttClient(wifiClient) {
    displayState = &i_displayState;
}

void Network::setup() {
    networkPreferences.begin("network", false);

    mqttClient.setServer(MQTT_SERVER, 1883);
    mqttClient.setCallback([&](char* topic, uint8_t* payload, unsigned int length) {
        this->mqttCallback(topic, payload, length);
    });
    mqttClient.setBufferSize(512);

    connect();
}

void connectRouteHandler(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {

}

void Network::startConnectAP() {
    Serial.println("starting ap");
    String apName = String(HOSTNAME) + "-AP";

    WiFi.softAP(apName);
    // dnsServer.start(53, "*", WiFi.softAPIP());

    // server.addHandler(new ApConnectHandler()).setFilter(ON_AP_FILTER);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send(200, "text/html", NETWORK_AP_HTML);
    });
    server.on("/a", HTTP_GET, [](AsyncWebServerRequest* req) {
        req->send(200, "text/html", "OIII");
    });
    server.on("/connect", HTTP_POST, [&](AsyncWebServerRequest* req) {
        Serial.println(req->params());
        String ssid = "";
        String password = "";
        Serial.println(req->hasParam("ssid", true));
        Serial.println(req->getParam(0)->name());
        Serial.println(req->getParam(0)->value());
        for (int i = 0; i < req->params(); ++i) {
            auto param = req->getParam(i);
            Serial.print("param: ");
            Serial.print(param->name());
            Serial.print(" -> ");
            Serial.println(param->value());
            if (param->name() == "ssid") {
                ssid = param->value();
            } else if (param->name() == "password") {
                password = param->value();
            }
        }

        if (ssid == "" || password == "") {
            req->send(400, "text/plain", "missing ssid or password");
            return;
        }

        this->networkPreferences.putString("ssid", ssid);
        this->networkPreferences.putString("password", password);

        // end ap server
        server.end();
        // dnsServer.stop();
        WiFi.mode(WIFI_MODE_STA);
        WiFi.enableAP(false);
        this->apControlEnabled = true;
        this->displayState->networkState.state = NET_STATE_CONNECTING;
        this->displayState->networkState.ssid = "";
        this->displayState->update();
        this->shouldConnect = true;

        req->send(200, "text/html", "done!");
    });

    server.onNotFound([](AsyncWebServerRequest* req) {
        req->send(404, "text/plain", "Not found");
    });


    server.begin();
    // WiFi.ap

    Serial.println("Server started");
    Serial.println(WiFi.softAPIP());
    displayState->networkState.state = NET_STATE_AP_CONNECT;
    displayState->networkState.ssid = WiFi.softAPSSID();
    displayState->networkState.ipAddres = WiFi.softAPIP();
    displayState->update();
    apControlEnabled = true;
    // callback(*this);
}

void Network::connectAPLoop() {
    // dnsServer.processNextRequest();
}

long lastMqttReconnectAttempt = 0;

void Network::connectMqtt() {
    long now = millis();
    if (now - lastMqttReconnectAttempt > 5000) {
        lastMqttReconnectAttempt = now;
        // Attempt to reconnect
        if (mqttClient.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
            Serial.println("mqtt connected!");

            mqttClient.subscribe(MQTT_TOPIC_SET_BUTTON_VALUE);
            mqttClient.subscribe(MQTT_TOPIC_SET_WATER_PREFS);
            mqttClient.subscribe(MQTT_TOPIC_SET_PUMP_SCHEDULE_PREFS);
            mqttClient.subscribe(MQTT_TOPIC_ACK);

            lastMqttReconnectAttempt = 0;
        }
    }
}

u_long lastDisplayStateUpdate = 0;
u_long lastSentTime = 0;

void Network::loop() {
    if (apControlEnabled) {
        connectAPLoop();
    } else if (WiFi.isConnected()) {
        if (!mqttClient.connected()) {
            connectMqtt();
        } else {
            mqttClient.loop();
        }
    }
    if (shouldConnect) {
        this->shouldConnect = false;
        this->connect();
    }

    const bool ackActive = (millis() - this->lastAckTime) < ACK_DURATION;
    const u_long sendInterval = ackActive ? SEND_INTERVAL_ACTIVE : SEND_INTERVAL;
    if (
        mqttClient.connected() &&
        displayState->lastUpdate != lastDisplayStateUpdate &&
        millis() - lastSentTime > sendInterval
    ) {
        Serial.print("----------- State update. Ack active: ");
        Serial.println(ackActive ? "true" : "false");
        lastDisplayStateUpdate = displayState->lastUpdate;
        lastSentTime = displayState->lastUpdate;
        sendState();
    }
}

void Network::sendState() {
    String message;
    message += "temp=" + String(displayState->temperatureValue);
    message += "&hum=" + String(displayState->humidity);
    message += "&wLevel=" + String(displayState->waterLevel);
    message += "&wDistance=" + String(displayState->waterDistance);
    message += "&wEptDist=" + String(displayState->waterEmptyDistance);
    message += "&wFullDist=" + String(displayState->waterFullDistance);
    message += "&pSchActive=" + String(displayState->pumpScheduleActive);
    message += "&pSchMinInterval=" + String(displayState->pumpScheduleMinInterval);
    message += "&pScheduleMaxOn=" + String(displayState->pumpScheduleMaxOn);
    message += "&pActiveLvl=" + String(displayState->pumpActiveLevel);
    message += "&pActive=" + String(displayState->pumpActive ? "true" : "false");
    message += "&lastPumpActiveTime=" + String(displayState->lastPumpActiveTime);
    message += "&pSchMinH=" + String(displayState->pumpScheduleMinHour);
    message += "&pSchMaxH=" + String(displayState->pumpScheduleMaxHour);

    for (auto button : displayState->buttons) {
        message += "&button" + String(button->index) + "=" + (button->active ? "true" : "false");
    }

    bool result = this->mqttClient.publish("weatherLite2/state", message.c_str());
    Serial.printf("Publishing mqtt state: %s\n", result ? "true" : "false");
}

void Network::connect() {
    // callback(*this);
    String wifiSSID = networkPreferences.getString("ssid", "");
    String wifiPassword = networkPreferences.getString("password", "");

    if (wifiSSID == "") {
        Serial.println("missing ssid");
        this->startConnectAP();
        return;
    }

    Serial.print("Connecting to wifi \"");
    Serial.print(wifiSSID);
    Serial.print("\" with password \"");
    Serial.print(wifiPassword);
    Serial.println("\"");

    this->displayState->networkState.state = NET_STATE_CONNECTING;
    this->displayState->update();

    WiFi.mode(WIFI_MODE_STA);
    WiFi.hostname(HOSTNAME);
    WiFi.begin(wifiSSID, wifiPassword);

    uint8_t tries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Wifi connect (");
        Serial.print(tries);
        Serial.println(")");

        delay(600);
        if (tries++ > 12) {
            Serial.println("could not connect");
            WiFi.disconnect();
            this->startConnectAP();
            break;
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to wifi");
        this->displayState->networkState.state = NET_STATE_CONNECTED;
        this->displayState->update();
    } else {
        this->startConnectAP();
        Serial.println("Could not connect to wifi");
    }
}

void Network::mqttCallback(String topic, const byte* payload, unsigned int length) {
    Serial.print("mqtt topic received: ");
    Serial.println(topic);

    String message;

    for (int i = 0; i < length; i++) {
        message += static_cast<char>(payload[i]);
    }

    Serial.print("message: ");
    Serial.println(message);
    Serial.println(message[0]);
    Serial.println(message[2]);

    if (topic == MQTT_TOPIC_ACK) {
        this->lastAckTime = millis();
    } else if (topic == MQTT_TOPIC_SET_BUTTON_VALUE) {
        uint8_t buttonIndex = String(message[0]).toInt();
        bool value = message.charAt(2) == 't';
        // bool value = true;
        // Serial.println(callbacks.size());
        // for (const auto callback : callbacks) {
        //     Serial.println("sending callback " + message);
        //     if (call)
        //     (*callback)(buttonIndex, value);
        // }
        // buttonCallback(buttonIndex, value);
        // Button* btn = displayState->buttons[buttonIndex];
        for (auto button : displayState->buttons) {
            Serial.printf("index: %d; buttonIndex: %d", button->index, buttonIndex);
            if (button->index == buttonIndex) {
                Serial.println("index matches");
                button->active = value;
                button->callback(*button);
            }
        }
        // Serial.println(displayState->buttons[0]);
        Serial.println(value);
        ESP_LOGW("D", displayState->buttons);
        ESP_LOGW("D", displayState->buttons[0]);

        // displayState->buttons[buttonIndex]->active = value;
        // displayState->buttons[buttonIndex]->callback(*displayState->buttons[buttonIndex]);
        // displayState->update();
    } else if (topic == MQTT_TOPIC_SET_WATER_PREFS) {
        handleWaterPrefCallback(message);
    } else if (topic == MQTT_TOPIC_SET_PUMP_SCHEDULE_PREFS) {
        handlePumpSchedulePrefCallback(message);
    }
}

void Network::handleWaterPrefCallback(const String& message) {
    int waterEmptyDistance = 0;
    String currentValueStr = "";

    for (int i = 0; i < message.length(); i++) {
        const char c = message.charAt(i);

        if (c == ';') {
            waterEmptyDistance = currentValueStr.toInt();
            currentValueStr = "";
        } else {
            currentValueStr += c;
        }
    }
    const int waterFullDistance = currentValueStr.toInt();

    Serial.printf("setting water pref: E: %d, F: %d\n", waterEmptyDistance, waterFullDistance);
    Preferences sensorsPreferences;
    sensorsPreferences.begin("sensors", false);

    sensorsPreferences.putInt(K_PREF_EMPTY_DISTANCE, waterEmptyDistance);
    sensorsPreferences.putInt(K_PREF_FULL_DISTANCE, waterFullDistance);
    // sensorsPreferences.putInt(, waterFullDistance);

    displayState->waterEmptyDistance = waterEmptyDistance;
    displayState->waterFullDistance = waterFullDistance;

    sensorsPreferences.end();
}

void Network::handlePumpSchedulePrefCallback(const String& message) {
    bool prefPumpScheduleActive = false;
    long prefPumpScheduleMinInterval = 0;
    long prefPumpScheduleMaxOn = 0;
    float prefPumpActiveLevel = 0;


    String currentValueStr = "";
    uint8_t dataIndex = 0;

    for (int i = 0; i < message.length(); i++) {
        const char c = message.charAt(i);

        if (c == ';') {
            if (dataIndex == 0) { // index 0
                prefPumpScheduleActive = currentValueStr == "1";
            } else if (dataIndex == 1) {
                prefPumpScheduleMinInterval = atol(currentValueStr.c_str());
            } else if (dataIndex == 2) {
                prefPumpScheduleMaxOn = atol(currentValueStr.c_str());
            }
            currentValueStr = "";
            dataIndex++;
        } else {
            currentValueStr += c;
        }
    }

    prefPumpActiveLevel = currentValueStr.toFloat();
    Serial.printf(" ---- %s", currentValueStr.c_str());
    Serial.printf(" ---- %f", prefPumpActiveLevel);

    Preferences sensorsPreferences;
    sensorsPreferences.begin("sensors", false);

    sensorsPreferences.putBool(K_PREF_PUMP_SCHEDULE_ACTIVE, prefPumpScheduleActive);
    sensorsPreferences.putLong(K_PREF_PUMP_SCHEDULE_MIN_INTERVAL, prefPumpScheduleMinInterval);
    sensorsPreferences.putLong(K_PREF_PUMP_SCHEDULE_MAX_ON, prefPumpScheduleMaxOn);
    sensorsPreferences.putFloat(K_PREF_PUMP_ACTIVE_LEVEL, prefPumpActiveLevel);

    displayState->pumpScheduleActive = prefPumpScheduleActive;
    displayState->pumpScheduleMinInterval = prefPumpScheduleMinInterval;
    displayState->pumpScheduleMaxOn = prefPumpScheduleMaxOn;
    displayState->pumpActiveLevel = prefPumpActiveLevel;

    sensorsPreferences.end();
}

void Network::setButtonCallback(void (*cb)(uint8_t buttonIndex, bool value)) {
    buttonCallback = cb;
    // callbacks.push_back(&callback);
}






