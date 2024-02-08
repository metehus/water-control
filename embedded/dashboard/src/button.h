//
// Created by methi on 26/11/2023.
//

#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
    typedef void (*ButtonCallback)(Button &button);

    void updateState(bool value);

public:
    int pin;
    uint8_t index;
    String name;
    bool active = false;
    bool pressing = false;
    ButtonCallback callback{};

    Button(int i_pin, uint8_t index, String name);

    void setup();
    void loop();
    void setCallback(void (*cb)(Button &button));
};



#endif //BUTTON_H
