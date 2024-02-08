#include "button.h"

Button::Button(const int i_pin, uint8_t i_index, String i_name) {
    pin = i_pin;
    index = i_index;
    name = i_name;
}

void Button::setup() {
    pinMode(pin, INPUT);


}

void Button::loop() {
    if (digitalRead(pin) == 0) {
        if (!pressing) {
            pressing = true;
            Serial.println("Pressing: " + pin);

            active = !active;

            callback(*this);
        }
    } else {
        pressing = false;
    }
}


void Button::setCallback(void (*cb)(Button& button)) {
    callback = cb;
}
