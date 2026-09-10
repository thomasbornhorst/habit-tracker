#ifndef IO_CONTROL_H
#define IO_CONTROL_H

#include <Arduino.h>
#include "light.h"

namespace IO {
    extern int buttonPressedIndex;
    extern Light redLED;
    extern Light greenLED;

    /**
     * @brief Initialize all buttons and lights
     * 
     */
    void initButtonsAndLEDs();

    /**
     * @brief Poll buttons for any updates to their state
     * 
     */
    void updateButtons();

    /**
     * @brief Poll LED states for any updates that need to happen
     * 
     */
    void updateLEDs();

    /**
     * @brief Get whether the shift button is pressed or not
     * 
     * @return True if shift button is currently pressed.
     * @return False if shift button is not currently pressed
     */
    bool isShiftButtonPressed();
}

#endif