#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Config.h>
#include "display.h"
#include <functional>
#include <Adafruit_GFX.h>

namespace {
    GxEPD2_BW<GxEPD2_DRIVER_CLASS, GxEPD2_DRIVER_CLASS::HEIGHT> display(
        GxEPD2_DRIVER_CLASS(Config::DISPLAY_CS_PIN, Config::DISPLAY_DC_PIN, Config::DISPLAY_RST_PIN, Config::DISPLAY_BUSY_PIN)
    );
}

namespace Display {
    void initDisplay() {
        display.init(115200);
        display.setRotation(0);
        display.setTextColor(GxEPD_BLACK);
    }

    void displayFullPage(std::function<void()> drawFn) {
        display.setFullWindow();
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);

            drawFn();
        } while (display.nextPage());

        display.hibernate();
    }

    void displayPartialPage(int x, int y, int w, int h, std::function<void()> drawFn) {
        display.setPartialWindow(x, y, w, h);
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);

            drawFn();
        } while (display.nextPage());
    }

    void displayStartupScreen() {
        displayFullPage([&]() {
            display.setFont(Config::titleFont);
            display.setCursor(50, 100);
            display.print("WELCOME!");
            
            display.setFont(Config::headerFont);
            display.setCursor(50, 200);
            display.print("Loading content");
        });
    }

    void drawCheckbox(int16_t leftX, int16_t topY) {
        display.drawRect(leftX, topY, Config::checkboxSize, Config::checkboxSize, GxEPD_BLACK);
    }

    void writeTextTopLeftAnchor(int16_t leftX, int16_t topY, String text, const GFXfont *font) {
        display.setFont(font);
        setCursorPositionsFromTextBounds(leftX, topY, text);
        display.print(text);
    }

    void writeTextMidLeftAnchor(int16_t leftX, int16_t midY, String text, const GFXfont *font) {
        display.setFont(font);
        setCursorPositionsFromTextBounds(leftX, midY, text, true);
        display.print(text);
    }

    void writeTextMidLeftAnchor(int16_t leftX, int16_t topY, uint16_t rowHeight, String text, const GFXfont *font) {
        int16_t midY = topY + (rowHeight / 2);
        writeTextMidLeftAnchor(leftX, midY, text, font);
    }

    void setCursorPositionsFromTextBounds(int16_t targetX, int16_t targetY, String text, bool anchorMidY, bool anchorMidX) {
        int16_t x1,y1;
        uint16_t w,h;
        display.getTextBounds(text,0,0,&x1,&y1,&w,&h);

        int16_t cursorX = targetX - x1 - (anchorMidX ? (w/2) : 0);
        int16_t cursorY = targetY - y1 - (anchorMidY ? (h/2) : 0);

        display.setCursor(cursorX, cursorY);
    }

    void drawTask(int16_t leftX, int16_t topY, String text, bool completed) {
        drawCheckbox(leftX,topY);

        leftX += Config::checkboxSize + Config::horizontalSpacing;
        
        writeTextMidLeftAnchor(leftX, topY, Config::checkboxSize, text, Config::bodyFont);

        // TODO: Implement status message for non-weekly things (maybe "Last Comp: ...")
        leftX = 225;
        writeTextMidLeftAnchor(leftX, topY, Config::checkboxSize, "| Weekly: 0/1", Config::smallFont);
        leftX = 370;
        drawCheckbox(leftX, topY);
    }

    void displayStateScreen() {
        displayFullPage([&]() {
            int16_t cursorX = 5;
            int16_t cursorY = 20;

            display.setFont(Config::headerFont);
            display.setCursor(cursorX, cursorY);
            display.print("Tasks! August 23rd, 2026");
            
            cursorX = Config::rowLeftX + (Config::checkboxSize / 2);
            cursorY = cursorY + 20;
            display.setFont(Config::smallFont);
            setCursorPositionsFromTextBounds(cursorX, cursorY, "DAILY", false, true);
            display.print("DAILY");

            cursorX = Config::rowLeftX;
            cursorY = cursorY + 20;

            drawTask(cursorX, cursorY, "Flashcards");
            cursorY += Config::checkboxSize + Config::rowSpacing;
            drawTask(cursorX, cursorY, "Cardio");
        });
    }
}