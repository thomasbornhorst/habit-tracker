#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "Fonts/FreeMonoBold24pt7b.h"
#include "Fonts/FreeMono18pt7b.h"
#include "Fonts/FreeMonoBold12pt7b.h"
#include "Fonts/FreeMono12pt7b.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Assets/FreeMono7pt7b.h"
#include "Assets/Icons.h"

namespace Config {
    // BUTTONS
    constexpr uint8_t BTN1_PIN = 32;
    constexpr uint8_t BTN2_PIN = 33;
    constexpr uint8_t BTN3_PIN = 25;
    constexpr uint8_t BTN4_PIN = 26;
    constexpr uint8_t BTN5_PIN = 27;
    constexpr int numMainButtons = 5;

    constexpr uint8_t SHIFT_BTN_PIN = 15;
    constexpr int numButtonOptions = 10; // 5 buttons + 1 shift => 10 total button options
    constexpr int numNonTaskButtons = 3;
    constexpr int maxTasksToShow = numButtonOptions - numNonTaskButtons;
    constexpr int nextPageButtonIndex = 7;
    constexpr int refreshDisplayButtonIndex = 8;
    constexpr int refreshDataButtonIndex = 9;

    constexpr int debounceDelay = 25;

    // LEDS
    constexpr uint8_t RED_LED_PIN = 13;
    constexpr uint8_t GREEN_LED_PIN = 14;

    // DISPLAY
    constexpr uint8_t DISPLAY_DIN_PIN = 23;
    constexpr uint8_t DISPLAY_SCK_PIN = 18;
    constexpr uint8_t DISPLAY_CS_PIN = 5;
    constexpr uint8_t DISPLAY_DC_PIN = 17;
    constexpr uint8_t DISPLAY_RST_PIN = 16;
    constexpr uint8_t DISPLAY_BUSY_PIN = 4;

    #define GxEPD2_DRIVER_CLASS GxEPD2_420_GDEY042T81

    // FONTS
    const GFXfont* const titleFont = &FreeMonoBold24pt7b;
    const GFXfont* const boldHeaderFont = &FreeMonoBold12pt7b;
    const GFXfont* const headerFont = &FreeMono12pt7b;
    const GFXfont* const bodyFont = &FreeMono12pt7b;
    const GFXfont* const smallFont = &FreeMono9pt7b;
    const GFXfont* const tinyFont = &FreeMono7pt7b;

    // NETWORK
    constexpr int minBetweenRefreshes = 10;

    // DISPLAY SIZES
    constexpr int titleOffsetX = 5;
    constexpr int titleOffsetY = 5;
    constexpr int titleRowSpacingX = 15;
    constexpr int titleSubtitleSpacing = 5;
    constexpr int iconSpacingX = 10;
    constexpr int iconLabelSpacing = 5;
    constexpr int iconOffsetRightX = 20;
    constexpr int iconOffsetY = 40;
    constexpr int buttonStrCheckboxSpacingX = 5;
    constexpr int maxTaskLabelWidth = 210;
    constexpr int taskDetailsOffsetX = 270;
    constexpr int iconSize = 16;
    constexpr int screenWidth = 400;
    constexpr int sectionSpacing = 30;
    constexpr int taskRowSpacing = 10;
    constexpr int checkboxSize = 20;
    constexpr int checkboxLabelSpacingX = 10;
    constexpr int taskOffsetX = 25;
    constexpr int checkmarkMainOffset = 5;
    constexpr int checkmarkMinorOffset = 3;
}

#endif