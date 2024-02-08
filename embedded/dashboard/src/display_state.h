//
// Created by methi on 28/01/2024.
//

#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H
#include <Arduino.h>
#include <button.h>


#define NET_STATE_CONNECTING 1
#define NET_STATE_CONNECTED 2
#define NET_STATE_AP_CONNECT 3

struct NetworkState {
    IPAddress ipAddres;
    String ssid;
    uint8_t state = NET_STATE_CONNECTING;
};

class DisplayState {
public:
    float temperatureValue = 0;
    float humidity = 0;
    int distance = 0;

    float waterLevel = 0;
    int waterDistance = 0;
    int waterFullDistance = 30;
    int waterEmptyDistance = 250;

    bool pumpScheduleActive = false;
    long pumpScheduleMinInterval = 24 * 60 * 60 * 1000; // 24h
    long pumpScheduleMaxOn = 1 * 60 * 1000; // 1h
    float pumpActiveLevel = 0.7;

    long lastPumpActiveTime = 0;
    bool pumpActive = false;

    NetworkState networkState = {};

    Button* buttons[3] = { nullptr, nullptr, nullptr };

    u_long lastUpdate = 10;

    void update();
};

#endif //DISPLAY_STATE_H
