#include <Arduino.h>
#include <Config.h>
#include "display.h"
#include "io_control.h"
#include "request_handler.h"
#include "network.h"
using namespace Config;

void setup() {
    Serial.begin(115200);

    initDisplay();
    initButtonsAndLEDs();
    redLED.turnOn();

    displayStartupScreen();
    
    if (connectToNetwork(5000)) {
        greenLED.turnOn();
        delay(100);
        greenLED.turnOff();
    }

    redLED.turnOff();
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