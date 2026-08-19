#include <Arduino.h>
#include <Config.h>
#include "io_control.h"
#include "button.h"
#include "light.h"

namespace {
    Button mainButtons[] = {
        Button(Config::BTN1_PIN),
        Button(Config::BTN2_PIN),
        Button(Config::BTN3_PIN),
        Button(Config::BTN4_PIN),
        Button(Config::BTN5_PIN)
    };

    Button shiftButton = Button(Config::SHIFT_BTN_PIN);
}

namespace IO {
    int buttonPressedIndex;
    Light greenLED = Light(Config::GREEN_LED_PIN);
    Light redLED = Light(Config::RED_LED_PIN);

    void initButtonsAndLEDs() {
        for (int i = 0; i < Config::numMainButtons; i++) {
            mainButtons[i].init();
        }

        shiftButton.init();

        greenLED.init();
        redLED.init();
    }

    // Updates button states
    // Updates buttonPressedIndex if any main buttons are newly pressed
    // If no main buttons newly pressed, sets buttonPressedIndex to -1
    void updateButtons() {
        buttonPressedIndex = -1;

        for (int i = 0; i < Config::numMainButtons; i++) {
            mainButtons[i].update();
            if (mainButtons[i].isNewlyPressed()) {
                buttonPressedIndex = i;
            }
        }

        shiftButton.update();
        if (buttonPressedIndex != -1 && shiftButton.isPressed()) {
            buttonPressedIndex += Config::numMainButtons;
        }
    }

    void updateLEDs() {
        greenLED.update();
        redLED.update();
    }

    bool isShiftButtonPressed() {
        return shiftButton.isPressed();
    }
}