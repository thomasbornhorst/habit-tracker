#include <Arduino.h>
#include "display.h"
#include "io_control.h"
#include "request_handler.h"
#include "network.h"
#include "state.h"

State mainState;

void flashGreenStatus() {
    IO::greenLED.turnOn();
    delay(100);
    IO::greenLED.turnOff();
}

void getNewState() {
    IO::redLED.turnOn();
    if (RequestHandler::getState(mainState)) {
        flashGreenStatus();
        mainState.rebuildButtons();
        Display::displayStateScreen(mainState);
    };
    IO::redLED.turnOff();
}

void setup() {
    Serial.begin(115200);

    Display::initDisplay();
    IO::initButtonsAndLEDs();
    IO::redLED.turnOn();

    Display::displayStartupScreen();
    
    if (Network::connectToNetwork(5000)) {
        flashGreenStatus();
    }

    getNewState();

    IO::redLED.turnOff();
}

void loop() {
    IO::updateButtons();
    IO::updateLEDs();

    if (RequestHandler::shouldRefreshData()) {
        getNewState();
    }

    if (IO::buttonPressedIndex != -1) {
        Serial.println(IO::buttonPressedIndex);
    }
}