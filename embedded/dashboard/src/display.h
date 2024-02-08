//
// Created by methi on 26/11/2023.
//

#ifndef DISPLAY_H
#define DISPLAY_H
#include <TFT_eSPI.h>
#include "display_state.h"


class Display {
    unsigned long last_active_time = millis();
    unsigned long fade_time = 60E3; // 60s

    unsigned long lastSensorsRenderTime = millis();
    unsigned long sensorsRenderInterval = 250; // 60s

public:
    bool active = true;
    TFT_eSPI tft = TFT_eSPI();
    DisplayState* state;

    explicit Display(DisplayState* i_display_state);

    void setup();
    void loop();
    void updateDisplayActive(bool value);
    void renderButtonStatus(bool buttonValue, String name, short index);
    void renderSensorsDisplay();

    void renderNetworkState();
};

#endif //DISPLAY_H
