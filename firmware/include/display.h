#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "state.h"

namespace Display {
    void initDisplay();

    void displayStartupScreen();

    void displayPartialPage(int x, int y, int w, int h, std::function<void()> drawFn);

    void displayStartupScreen();

    void drawCheckbox(int16_t leftX, int16_t topY);

    void writeTextTopLeftAnchor(int16_t leftX, int16_t topY, String text, const GFXfont *font);

    void writeTextMidLeftAnchor(int16_t leftX, int16_t midY, String text, const GFXfont *font);

    void writeTextMidLeftAnchor(int16_t leftX, int16_t topY, uint16_t rowHeight, String text, const GFXfont *font);

    void setCursorPositionsFromTextBounds(int16_t leftX, int16_t targetY, String text, bool anchorMidY = false, bool anchorMidX = false);

    void drawTask(Task& task);

    void drawHeader(State& state, int16_t x, int16_t y);

    void drawTaskCheckboxAsCompleted(Task& task);

    void drawTaskListHeader(int16_t x, int16_t y);

    void displayUpdateTaskCompletionStatus(Task& task);

    void displayStateScreen(State& state);
}

#endif