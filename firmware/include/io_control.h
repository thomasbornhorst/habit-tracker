#ifndef IO_CONTROL_H
#define IO_CONTROL_H

#include <Arduino.h>
#include "light.h"

extern Light redLED;
extern Light greenLED;

void initButtonsAndLEDs();

void updateButtons();

bool isAnyMainButtonNewlyPressed();

bool isShiftButtonPressed();

#endif