//
// Created by methi on 28/11/2023.
//

#include "sensors.h"

#include <keys.h>

#include "utils.h"
#include <Wire.h>

Sensors::Sensors(DisplayState* i_displayState) {
    displayState = i_displayState;
}

void Sensors::setup() {
    pinMode(JSN_ECHO_PIN, INPUT);
    pinMode(JSN_TRIG_PIN, OUTPUT);

    bool bmeStatus = bmeSensor.begin(0x76);
    if (!bmeStatus) {
        Serial.println("Could not connect to bme280 sensor!");
        // display->tft.fillRect(100, 10, 10, 10, TFT_RED);
    }

    sensorsPreferences.begin("sensors", true);

    const int prefWaterEmptyDistance = sensorsPreferences.getInt(K_PREF_EMPTY_DISTANCE, 30);
    const int prefWaterFullDistance = sensorsPreferences.getInt(K_PREF_FULL_DISTANCE, 250);
    const int prefPumpScheduleActive = sensorsPreferences.getBool(K_PREF_PUMP_SCHEDULE_ACTIVE, false);
    const int prefPumpScheduleMinInterval = sensorsPreferences.getLong(K_PREF_PUMP_SCHEDULE_MIN_INTERVAL, 24 * 60 * 60 * 1000); // 24h
    const int prefPumpScheduleMaxOn = sensorsPreferences.getLong(K_PREF_PUMP_SCHEDULE_MAX_ON, 1 * 60 * 1000); // 1h
    const float prefPumpActiveLevel = sensorsPreferences.getFloat(K_PREF_PUMP_ACTIVE_LEVEL, 0.7);

    displayState->waterEmptyDistance = prefWaterEmptyDistance;
    displayState->waterFullDistance = prefWaterFullDistance;
    displayState->pumpScheduleActive = prefPumpScheduleActive;
    displayState->pumpScheduleMinInterval = prefPumpScheduleMinInterval;
    displayState->pumpScheduleMaxOn = prefPumpScheduleMaxOn;
    displayState->pumpActiveLevel = prefPumpActiveLevel;

    sensorsPreferences.end();
}

int Sensors::measureDistance() {
    Serial.println("Measuring distance");
    digitalWrite(JSN_TRIG_PIN, LOW);
    delayMicroseconds(5);

    digitalWrite(JSN_TRIG_PIN, HIGH);

    // return 20;
    delayMicroseconds(20);
    digitalWrite(JSN_TRIG_PIN, LOW);

    u_long duration = pulseIn(JSN_ECHO_PIN, HIGH);

    return duration * 0.034 / 2;
}


void Sensors::updateEnvironment() {
    const int distance = measureDistance();

    const int fullDistance = displayState->waterFullDistance;
    const int emptyDistance = displayState->waterEmptyDistance;
    // TODO: essa funcao nao precisa de um lerp ja que a faixa de retorno é entre 0 e 1
    const float waterLevel = lerp(emptyDistance, fullDistance, 0, 1, distance);

    displayState->humidity = bmeSensor.readHumidity();
    displayState->temperatureValue = bmeSensor.readTemperature();
    displayState->distance = distance;
    displayState->waterLevel = waterLevel;
    displayState->waterDistance = distance;
    displayState->update();
}

void Sensors::checkWaterPumpSchedule() {
    auto pumpButton = displayState->buttons[0];

    // se o tempo entre a ultima vez que ativou até o tempo atual for MAIOR que o minimo pra ligar denovo
    Serial.printf("Checking pump availability (lastActiveDuration: %d, minInterval: %d) \n", millis() - displayState->lastPumpActiveTime, displayState->pumpScheduleMinInterval);
    if (millis() - displayState->lastPumpActiveTime > displayState->pumpScheduleMinInterval) {
        Serial.printf(
            "Checking for water level... (now: %f / targetLevel: %f / active: %s) \n",
            displayState->waterLevel, displayState->pumpActiveLevel, displayState->pumpActive ? "true" : "false"
        );
        if (displayState->waterLevel >= displayState->pumpActiveLevel && !displayState->pumpActive) {
            displayState->pumpActive = true;
            displayState->lastPumpActiveTime = millis();
            pumpButton->active = true;
            pumpButton->callback(*pumpButton);

            displayState->update();
        }
    } else if ((displayState->waterLevel <= 0.2 && displayState->pumpActive) || millis() - displayState->lastPumpActiveTime > displayState->pumpScheduleMaxOn) {
        displayState->pumpActive = false;
        pumpButton->active = false;
        pumpButton->callback(*pumpButton);

        displayState->update();
    }
}

void Sensors::loop() {
    if (millis() - lastMeasureTime > MEASURE_INTERVAL) {
        updateEnvironment();

        if (displayState->pumpScheduleActive) {
            checkWaterPumpSchedule();
        }

        lastMeasureTime = millis();
    }
}


