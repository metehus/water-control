//
// Created by methi on 25/01/2024.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include "display_state.h"
#include "keys.h"

#define HOSTNAME "Controle-Bombas"
#define SEND_INTERVAL 1000


class Network {
public:
    typedef void (*NetworkCallback)(uint8_t buttonIndex, bool value);
    // using NetworkCallback = std::function<void(uint8_t buttonIndex, bool value)>;

    DisplayState* displayState;

    WiFiClient wifiClient;

    // DNSServer dnsServer;
    AsyncWebServer server;
    PubSubClient mqttClient;

    // std::vector<NetworkCallback*> callbacks;
    NetworkCallback buttonCallback{};


    Preferences networkPreferences;
    bool apControlEnabled = false;
    bool shouldConnect = false;

    explicit Network(DisplayState& i_displayState);

    void connect();
    void startConnectAP();
    void connectAPLoop();
    void connectMqtt();

    void mqttCallback(String topic, const byte* payload, unsigned int length);
    void handleButtonCallback();
    void handleWaterPrefCallback(const String& message);
    void handlePumpSchedulePrefCallback(const String& message);

    void setButtonCallback(void (*cb)(uint8_t buttonIndex, bool value));
    void sendState();

    void setup();
    void loop();
};



#endif //NETWORK_H
