#ifndef IO_CONTROL_H
#define IO_CONTROL_H

#include <Arduino.h>
#include "light.h"

extern int buttonPressedIndex;
extern Light redLED;
extern Light greenLED;

void initButtonsAndLEDs();

void updateButtons();

void updateLEDs();

bool isShiftButtonPressed();

#endif