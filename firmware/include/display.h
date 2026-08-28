#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "state.h"

namespace Display {
    struct Coords { int16_t x, y; };

    enum class TextAnchorX { Left, Right, Middle };
    enum class TextAnchorY { Top, Middle };

    struct TextAnchors { 
        TextAnchorX textAnchorX; 
        TextAnchorY textAnchorY; 
    };

    void initDisplay();

    void displayFullPage(std::function<void()> drawFn);

    void displayPartialPage(int x, int y, int w, int h, std::function<void()> drawFn);

    Coords writeTextMidLeftAnchor(int16_t leftX, int16_t topY, uint16_t rowHeight, String text, const GFXfont *font, uint16_t maxW = -1);

    Coords writeTextMidTopAnchor(int16_t leftX, int16_t topY, uint16_t rowWidth, String text, const GFXfont *font, uint16_t maxW = -1);

    Coords writeText(int16_t targetX, int16_t targetY, String text, const GFXfont *font, TextAnchors textAnchors = {TextAnchorX::Left,TextAnchorY::Top}, uint16_t maxW = -1);

    Coords drawHeader(State& state, int16_t x, int16_t y);

    Coords drawSubHeader(State& state, int16_t x, int16_t y);

    String getButtonStr(int btnIndex);

    void drawSpecialButtons(State& state, int16_t x, int16_t y);

    void drawIcon(int16_t x, int16_t y, const uint8_t *bitmap, int btnIndex = -1);

    Coords drawTaskListHeader(int16_t x, int16_t y);

    Coords drawCheckbox(int16_t leftX, int16_t topY);
    
    void drawTaskCheckboxAsCompleted(Task& task);

    Coords drawTask(Task& task);

    void displayStartupScreen();

    void displayUpdateTaskCompletionStatus(Task& task);

    void displayStateScreen(State& state);
}

#endif