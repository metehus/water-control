//
// Created by methi on 28/01/2024.
//
#include "display_state.h"

void DisplayState::update() {
    this->lastUpdate = millis();
    Serial.println(this->lastUpdate);
}
