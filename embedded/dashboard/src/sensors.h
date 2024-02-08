//
// Created by methi on 28/11/2023.
//

#ifndef SENSORS_H
#define SENSORS_H

#include "utils.h"
#include <display.h>
#include <Adafruit_BME280.h>
#include <Preferences.h>

#define JSN_TRIG_PIN 12
#define JSN_ECHO_PIN 14

#define MEASURE_INTERVAL 500 // 0.5s

class Sensors {
    DisplayState* displayState;

    long lastMeasureTime = 0;

    Adafruit_BME280 bmeSensor;

public:
    explicit Sensors(DisplayState* i_displayState);

    Preferences sensorsPreferences;

    int measureDistance();

    void updateEnvironment();
    void checkWaterPumpSchedule();

    void setup();
    void loop();
};



#endif //SENSORS_H
