#include <Arduino.h>
#include "main.h"
#include "button.h"
#include "display.h"
#include "sensors.h"
#include "network.h"

#define BUTTON1_PIN 36
#define BUTTON2_PIN 39
#define BUTTON3_PIN 34

#define BUTTON1_RELAY_PIN 25
#define BUTTON2_RELAY_PIN 26
#define BUTTON3_RELAY_PIN 27

Button button1(BUTTON1_PIN, 0, "Bomba Mina");
Button button2(BUTTON2_PIN, 1, "Bomba 1");
Button button3(BUTTON3_PIN, 2, "Bomba 2");

Button buttons[] = { button1, button2, button3 };
int buttonRelays[] = { BUTTON1_RELAY_PIN, BUTTON2_RELAY_PIN, BUTTON3_RELAY_PIN };

DisplayState displayState = *(new DisplayState());

Display display(&displayState);
Sensors sensors(&displayState);
Network network(displayState);

TaskHandle_t MainTask;
TaskHandle_t SensorsTask;

void buttonCallback(Button button) {
    digitalWrite(buttonRelays[button.index], !button.active);
    display.updateDisplayActive(true);
    displayState.update();
}


void mainTask(void* params) {
    for (;;) {
        display.loop();

        button1.loop();
        button2.loop();
        button3.loop();
        delay(10);
    }
}

void sensorTask(void* params) {
    network.setup();


    Serial.println("Starting sensor loop");
    for (;;) {
        sensors.loop();
        network.loop();
        delay(10);
    }
}



void setup() {
    Serial.begin(9600);
    display.setup();

    sensors.setup();

    digitalWrite(BUTTON1_RELAY_PIN, HIGH);
    digitalWrite(BUTTON2_RELAY_PIN, HIGH);
    digitalWrite(BUTTON3_RELAY_PIN, HIGH);

    pinMode(BUTTON1_RELAY_PIN, OUTPUT);
    pinMode(BUTTON2_RELAY_PIN, OUTPUT);
    pinMode(BUTTON3_RELAY_PIN, OUTPUT);

    button1.setup();
    button2.setup();
    button3.setup();
    button1.setCallback([](Button &button) {
        buttonCallback(button);
    });

    button2.setCallback([](Button &button) {
        buttonCallback(button);
    });

    button3.setCallback([](Button &button) {
        buttonCallback(button);
    });

    displayState.buttons[0] = &button1;
    displayState.buttons[1] = &button2;
    displayState.buttons[2] = &button3;

    // disableCore0WDT();

    xTaskCreatePinnedToCore(mainTask, "MainTask", 20000, nullptr, 8, &MainTask, 0);
    xTaskCreatePinnedToCore(sensorTask, "SensorTask", 20000, nullptr, 4, &SensorsTask, 1);
}



void loop() {
    delay(100);
}

