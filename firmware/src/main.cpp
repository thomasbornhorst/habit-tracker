#include <Arduino.h>
#include <Config.h>
#include "display.h"
#include "io_control.h"
using namespace Config;

void setup() {
    Serial.begin(115200);

    initDisplay();
    initButtonsAndLEDs();

    displayStartupScreen();
}

void loop() {
    updateButtons();

    if (isShiftButtonPressed()) {
        greenLED.turnOn();
    } else {
        greenLED.turnOff();
    }
}