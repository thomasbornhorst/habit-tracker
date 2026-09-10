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

    /**
     * @brief Initialize button pin
     * 
     */
    void init() {
        pinMode(pin, INPUT_PULLUP);
    }

    /**
     * @brief Update button state based on current reading
     * 
     */
    void update() {
        bool newReading = (digitalRead(pin) == LOW);

        previouslyPressed = currentlyPressed;

        // button state has changed
        if (newReading != reading) {
            lastStateChange = millis();
            reading = newReading;
        }

        // debouncing
        if (reading != currentlyPressed) {
            if ((millis() - lastStateChange) > Config::debounceDelay) {
                currentlyPressed = reading;
            }
        }
    }

    /**
     * @brief Get current status of if the button is pressed or not
     * 
     * @return True if button is pressed.
     * @return False otherwise
     */
    bool isPressed() const {
        return currentlyPressed;
    }

    /**
     * @brief Get current status of if the button newly has a status of pressed
     * 
     * @return True if considered pressed in this update, but was not pressed in last update.
     * @return False otherwise
     */
    bool isNewlyPressed() const {
        return (!previouslyPressed && currentlyPressed);
    }
};

enum class ButtonAction { ToggleTask, RefreshData, RefreshDisplay, NextPage, None };

struct ButtonBinding {
    ButtonAction action;
    int taskIndex;

    explicit ButtonBinding() {}

    explicit ButtonBinding(ButtonAction buttonAction): action(buttonAction) {
        taskIndex = -1;
    }

    explicit ButtonBinding(ButtonAction buttonAction, int taskIndex): action(buttonAction), taskIndex(taskIndex) {}
};

#endif