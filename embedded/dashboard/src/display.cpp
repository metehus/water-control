#include <display.h>
#include <TJpg_Decoder.h>
#include <TFT_eSPI.h>
#include "network-assets.h"

#define FADED_PWM 30
#define ACTIVE_PWM 220
#define LED_PIN 19
#define LED_PIN_CHANNEL 0

Display::Display(DisplayState* i_display_state) {
    this->state = i_display_state;
}

TFT_eSPI* tftInstance;

void Display::setup() {
    tftInstance = &tft;
    tft.begin();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);

    TJpgDec.setSwapBytes(true);

    TJpgDec.setCallback([](int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* data) {
        if (y >= tftInstance->height()) return false;

        tftInstance->pushImage(x, y, w, h, data);

        return true;
    });

    ledcAttachPin(LED_PIN, LED_PIN_CHANNEL);
    ledcSetup(0, 1000, 8);
    updateDisplayActive(true);

    renderButtonStatus(false, "Bomba Mina", 0);
    renderButtonStatus(false, "Bomba 1", 1);
    renderButtonStatus(false, "Bomba 2", 2);
}

void Display::updateDisplayActive(const bool value) {
    active = value;
    ledcWrite(LED_PIN_CHANNEL, active ? ACTIVE_PWM : FADED_PWM);
    if (active) {
        last_active_time = millis();
    }
}

void Display::renderButtonStatus(bool buttonValue, String name, short index) {
    int margin = 2;
    int buttonOuterWidth = tft.width() / 3;
    int buttonWidth = buttonOuterWidth - margin * 2;
    int buttonHeight = 120 - margin * 2;

    int x = buttonOuterWidth * index + margin;
    int y = tft.height() - buttonHeight;


    uint32_t color = buttonValue ? TFT_GREEN : TFT_RED;

    tft.fillRoundRect(x, y, buttonWidth, buttonHeight, 2, color);
    tft.setTextSize(3);
    tft.setTextColor(TFT_BLACK, color);
    tft.drawCentreString(buttonValue ? "ON" : "OFF", x + buttonWidth / 2, y + 10, 2);
    tft.setTextSize(1);
    tft.drawCentreString(name, x + buttonWidth / 2, y + 80, 2);
}

uint8_t waterLevelStroke = 2;
uint16_t waterLevelWidth = 100;
uint16_t waterLevelHeight = 140;
uint16_t waterLevelX = 240 - waterLevelWidth - 38;
uint16_t waterLevelY = 30;

uint16_t waterLevelInnerHeight = waterLevelHeight - waterLevelStroke;
uint16_t waterLevelInnerWidth = waterLevelWidth - waterLevelStroke * 2;

void Display::renderSensorsDisplay() {
    String temperatureString = String(state->temperatureValue) + " c     ";
    String humidityString = String(state->humidity) + " %     ";

    const int x = 18;
    const int startY = 50;

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);

    tft.drawString(temperatureString, x, startY, 2);
    tft.drawString(humidityString , x, startY + 24, 2);

    // water level
    double clampedLevel = max(0.0f, min(1.0f, state->waterLevel));

    const String levelString = String((uint8_t)(state->waterLevel * 100)) + "%";
    const String distanceString = "      (" + String(state->distance) + "cm)      ";

    // left line
    tft.fillRect(waterLevelX, waterLevelY, waterLevelStroke, waterLevelHeight, TFT_WHITE);
    // bottom line
    tft.fillRect(waterLevelX, waterLevelY + waterLevelHeight - waterLevelStroke, waterLevelWidth, waterLevelStroke, TFT_WHITE);
    // right line
    tft.fillRect(waterLevelX + waterLevelWidth - waterLevelStroke, waterLevelY, waterLevelStroke, waterLevelHeight, TFT_WHITE);

    const uint16_t waterFillHeight = clampedLevel * waterLevelInnerHeight;
    const uint16_t blankFillHeight = (1.0 - clampedLevel) * waterLevelInnerHeight;

    tft.fillRect(waterLevelX + waterLevelStroke, waterLevelY, waterLevelInnerWidth, blankFillHeight, TFT_BLACK);
    tft.fillRect(waterLevelX + waterLevelStroke, waterLevelY + blankFillHeight + 1, waterLevelInnerWidth, waterFillHeight, TFT_BLUE);

    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString(levelString, waterLevelX + waterLevelWidth / 2, waterLevelY + waterLevelHeight / 2, 2);

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCentreString(distanceString, waterLevelX + waterLevelWidth / 2, waterLevelY - 20, 2);
}

int16_t networkAreaX = 10;
int16_t networkAreaY = 100;


void Display::renderNetworkState() {
    auto networkState = state->networkState;

    // pump active
    tft.fillCircle(6 + 24 + 20 / 2, 6 + 20 / 2, 8, state->pumpScheduleActive ? TFT_GREEN : TFT_RED);

   if (networkState.state == NET_STATE_AP_CONNECT) {
       auto apData = state->networkState;
       TJpgDec.drawJpg(6, 6, AP_CONFIG_JPG, sizeof(AP_CONFIG_JPG));

       tft.setTextColor(TFT_RED, TFT_BLACK);
       tft.setTextSize(1);
       tft.setCursor(0, networkAreaY);
       tft.println("WiFi nao conectado");
       tft.println("SSID:");
       tft.println(apData.ssid);
       tft.println("IP:");
       tft.println(apData.ipAddres.toString());
   } else if (networkState.state == NET_STATE_CONNECTED) {
       TJpgDec.drawJpg(6, 6, CONNECTED_JPG, sizeof(CONNECTED_JPG));

       tft.setTextColor(TFT_RED, TFT_BLACK);
       tft.setTextSize(1);
       tft.setCursor(0, networkAreaY);
       tft.fillRect(0, networkAreaY, waterLevelX - 1, 100, TFT_BLACK);
   }else if (networkState.state == NET_STATE_CONNECTING) {
       TJpgDec.drawJpg(6, 6, DISCONNECTED_JPG, sizeof(DISCONNECTED_JPG));

       tft.setTextColor(TFT_RED, TFT_BLACK);
       tft.setTextSize(1);
       tft.setCursor(0, networkAreaY);
       tft.fillRect(0, networkAreaY, waterLevelX - 1, 100, TFT_BLACK);
       tft.println("Conectando...");
   }
}

u_long lastStateUpdate = 0;

void Display::loop() {
    if ((millis() - last_active_time) > fade_time && active) {
        updateDisplayActive(false);
    }

    if (millis() - lastSensorsRenderTime > sensorsRenderInterval) {
        renderSensorsDisplay();

        lastSensorsRenderTime = millis();
    }

    if (state->lastUpdate != lastStateUpdate) {
        Serial.print("[ST] ");
        Serial.print(lastStateUpdate);
        lastStateUpdate = state->lastUpdate;
        Serial.println(" - Display state changed!");
        this->renderNetworkState();
        for (auto button : state->buttons) {
            renderButtonStatus(button->active, button->name, button->index);
        }
    }
}
