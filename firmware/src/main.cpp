#include <Arduino.h>
#include <Config.h>
#include "display.h"
#include "io_control.h"
#include "request_handler.h"
using namespace Config;

void setup() {
    Serial.begin(115200);

    initDisplay();
    initButtonsAndLEDs();

    displayStartupScreen();

    redLED.startBlinking(250, 1000);
}

void loop() {
    updateButtons();
    updateLEDs();

    if (shouldRefreshData()) {
        greenLED.startBlinking(250, 5000);
        refreshData();
    }

    if (buttonPressedIndex != -1) {
        Serial.println(buttonPressedIndex);
    }
}