#include <Arduino.h>
#include "display.h"
#include "io_control.h"
#include "request_handler.h"
#include "network.h"

void setup() {
    Serial.begin(115200);

    Display::initDisplay();
    IO::initButtonsAndLEDs();
    IO::redLED.turnOn();

    Display::displayStartupScreen();
    
    if (Network::connectToNetwork(5000)) {
        IO::greenLED.turnOn();
        delay(100);
        IO::greenLED.turnOff();
    }

    RequestHandler::getState();

    IO::redLED.turnOff();
    Display::displayStateScreen();
}

void loop() {
    IO::updateButtons();
    IO::updateLEDs();

    if (RequestHandler::shouldRefreshData()) {
        IO::greenLED.startBlinking(250, 5000);
        RequestHandler::refreshData();
    }

    if (IO::buttonPressedIndex != -1) {
        Serial.println(IO::buttonPressedIndex);
    }
}