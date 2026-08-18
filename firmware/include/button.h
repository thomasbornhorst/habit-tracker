#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Config.h"

struct Button {
    uint8_t pin;
    bool previouslyPressed = false;
    bool currentlyPressed = false;

    bool reading = false;
    unsigned long lastStateChange;

    explicit Button(uint8_t pin) : pin(pin) {}

    void init() {
        pinMode(pin, INPUT_PULLUP);
    }

    void update() {
        bool newReading = (digitalRead(pin) == LOW);

        // button state has changed
        if (newReading != reading) {
            lastStateChange = millis();
            reading = newReading;
        }

        // debouncing
        if (reading != currentlyPressed) {
            if ((millis() - lastStateChange) > Config::debounceDelay) {
                previouslyPressed = currentlyPressed;
                currentlyPressed = reading;
            }
        }
    }

    bool isPressed() {
        return currentlyPressed;
    }

    bool isNewlyPressed() {
        return (!previouslyPressed && currentlyPressed);
    }
};

#endif