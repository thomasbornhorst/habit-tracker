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
    int numMainButtons = 5;

    Button shiftButton = Button(SHIFT_BTN_PIN);

    int buttonPressedIndex;
}

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

void updateButtons() {
    for (int i = 0; i < numMainButtons; i++) {
        mainButtons[i].update();
    }

    shiftButton.update();
}

bool isAnyMainButtonNewlyPressed() {
    bool isAnyButtonPressed = false;

    for (int i = 0; i < numMainButtons; i++) {
        if (mainButtons[i].isNewlyPressed()) {
            isAnyButtonPressed = true;
            buttonPressedIndex = i;
            break;
        }
    }
    
    return isAnyButtonPressed;
}

bool isShiftButtonPressed() {
    return shiftButton.isPressed();
}