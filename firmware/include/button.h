#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

struct Button {
    uint8_t pin;
    bool previouslyPressed = false;
    bool currentlyPressed = false;

    explicit Button(uint8_t pin) : pin(pin) {}

    void init() {
        pinMode(pin, INPUT_PULLUP);
    }

    void update() {
        previouslyPressed = currentlyPressed;
        currentlyPressed = (digitalRead(pin) == LOW);
    }

    bool isPressed() {
        return currentlyPressed;
    }

    bool isNewlyPressed() {
        return (!previouslyPressed && currentlyPressed);
    }
};

#endif