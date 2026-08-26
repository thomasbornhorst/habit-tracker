#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Config.h>
#include "display.h"
#include <functional>
#include <Adafruit_GFX.h>
#include "state.h"
#include "task.h"

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

        display.hibernate();
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

    void drawTask(Task& task) {
        int16_t leftX = task.xStartCoord;
        int16_t topY = task.yStartCoord;

        //TODO: Deal with overflowing strings & better positioning
        drawCheckbox(leftX,topY);
        if (task.isCompleted) {
            drawTaskCheckboxAsCompleted(task);
        }

        leftX += Config::checkboxSize + Config::horizontalSpacing;
        
        writeTextMidLeftAnchor(leftX, topY, Config::checkboxSize, task.label, Config::bodyFont);

        leftX = 300;
        writeTextMidLeftAnchor(leftX, topY, Config::checkboxSize, task.getTaskDetailsString(), Config::smallFont);
    }

    void drawHeader(State& state, int16_t x, int16_t y) {
        display.setFont(Config::headerFont);
        display.setCursor(x, y);
        display.print("Tasks! " + state.dateString);
    }

    void drawTaskCheckboxAsCompleted(Task& task) {
        int offset = 2;
        int16_t intersectionX = task.xStartCoord + (Config::checkboxSize / 2);
        int16_t intersectionY = task.yStartCoord + (Config::checkboxSize - 4);

        display.drawLine((task.xStartCoord + 5),(task.yStartCoord + 5),intersectionX,intersectionY,GxEPD_BLACK);
        display.drawLine(intersectionX,intersectionY,(task.xStartCoord + Config::checkboxSize + 3),(task.yStartCoord - 5),GxEPD_BLACK);
    }

    void drawTaskListHeader(int16_t x, int16_t y) {
        display.setFont(Config::smallFont);
        setCursorPositionsFromTextBounds(x, y, "TODAY", false, true);
        display.print("TODAY");
    }

    void displayUpdateTaskCompletionStatus(Task& task) {
        int16_t x, y, w, h;
        x = task.xStartCoord;
        y = task.yStartCoord - 5;
        w = Config::checkboxSize + 3;
        h = Config::checkboxSize + 5;
        displayPartialPage(x, y, w, h, [&]() {
            display.fillRect(x, y, w, h, GxEPD_WHITE);
            drawCheckbox(task.xStartCoord, task.yStartCoord);
            if (task.isCompleted) {
                drawTaskCheckboxAsCompleted(task);
            }
        });
    }

    void displayStateScreen(State& state) {
        displayFullPage([&]() {
            int16_t cursorX = 5;
            int16_t cursorY = Config::sectionSpacing;
            drawHeader(state, cursorX, cursorY);
            
            cursorX = Config::rowLeftX + (Config::checkboxSize / 2);
            cursorY += Config::sectionSpacing;
            drawTaskListHeader(cursorX, cursorY);

            cursorX = Config::rowLeftX;
            cursorY += Config::sectionSpacing;
            for (int i = 0; i < state.tasks.size(); i++) {
                Task& task = state.tasks[i];
                task.xStartCoord = cursorX;
                task.yStartCoord = cursorY;

                drawTask(task);

                cursorY += Config::checkboxSize + Config::taskRowSpacing;
            }
        });
    }
}