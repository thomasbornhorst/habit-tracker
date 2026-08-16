#include <Arduino.h>
#include <Config.h>
#include "io_control.h"
#include "button.h"
#include "light.h"
using namespace Config;

namespace {
    Button mainButtons[] = {
        Button(BTN1_PIN),
        Button(BTN2_PIN),
        Button(BTN3_PIN),
        Button(BTN4_PIN),
        Button(BTN5_PIN)
    };

    Button shiftButton = Button(SHIFT_BTN_PIN);
}

int buttonPressedIndex;
Light greenLED = Light(GREEN_LED_PIN);
Light redLED = Light(RED_LED_PIN);

void initButtonsAndLEDs() {
    for (int i = 0; i < numMainButtons; i++) {
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

    for (int i = 0; i < numMainButtons; i++) {
        mainButtons[i].update();
        if (mainButtons[i].isNewlyPressed()) {
            buttonPressedIndex = i;
        }
    }

    shiftButton.update();
    if (buttonPressedIndex != -1 && shiftButton.isPressed()) {
        buttonPressedIndex += numMainButtons;
    }
}

void updateLEDs() {
    greenLED.update();
    redLED.update();
}

bool isShiftButtonPressed() {
    return shiftButton.isPressed();
}