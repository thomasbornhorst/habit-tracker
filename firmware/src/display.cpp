#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Config.h>
#include "display.h"
#include <functional>
#include <Adafruit_GFX.h>
#include "state.h"
#include "task.h"
#include "Assets/Icons.h"

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

    Coords writeTextMidLeftAnchor(int16_t leftX, int16_t topY, uint16_t rowHeight, String text, const GFXfont *font, uint16_t maxW) {
        int16_t midY = topY + (rowHeight / 2);
        return writeText(leftX, midY, text, font, {TextAnchorX::Left, TextAnchorY::Middle}, maxW);
    }

    Coords writeTextMidTopAnchor(int16_t leftX, int16_t topY, uint16_t rowWidth, String text, const GFXfont *font, uint16_t maxW) {
        int16_t midX = leftX + (rowWidth / 2);
        return writeText(midX, topY, text, font, {TextAnchorX::Middle, TextAnchorY::Top}, maxW);
    }

    // default: anchors top left
    Coords writeText(int16_t targetX, int16_t targetY, String text, const GFXfont *font, TextAnchors textAnchors, uint16_t maxW) {
        int16_t x1, y1;
        uint16_t w, h;

        display.setFont(font);
        display.getTextBounds(text,0,0,&x1,&y1,&w,&h);

        if (maxW > 0 && w > maxW) {
            uint16_t dotsW;
            display.getTextBounds("..",0,0,&x1,&y1,&dotsW,&h);
            if (dotsW > maxW) {
                return { 0, 0 };
            }

            while ((w / 2) > maxW) {
                text = text.substring(0, (text.length() / 2));
                display.getTextBounds(text,0,0,&x1,&y1,&w,&h);
            }

            String textWithoutDots = text;
            while (w > maxW) {
                textWithoutDots = textWithoutDots.substring(0, (textWithoutDots.length() - 1));
                text = textWithoutDots + "..";
                display.getTextBounds(text,0,0,&x1,&y1,&w,&h);
            }
        }

        int16_t xOffset = 0;
        if (textAnchors.textAnchorX == TextAnchorX::Middle) {
            xOffset = w/2;
        } else if (textAnchors.textAnchorX == TextAnchorX::Right) {
            xOffset = w;
        }

        int16_t yOffset = 0;
        if (textAnchors.textAnchorY == TextAnchorY::Middle) {
            yOffset = h/2;
        }

        int16_t cursorX = targetX - x1 - xOffset;
        int16_t cursorY = targetY - y1 - yOffset;
        display.setCursor(cursorX, cursorY);
        display.print(text);

        return { static_cast<int16_t>(cursorX + w), cursorY };
    }

    // input: top left coords
    // output: bottom right coords
    Coords drawHeader(State& state, int16_t x, int16_t y) {
        Coords coords = writeText(x, y, "Tasks!", Config::boldHeaderFont);

        x = coords.x + Config::titleRowSpacingX;
        x = writeText(x, y, state.dateString, Config::headerFont).x;
        y = coords.y;

        return { x, y };
    }

    Coords drawSubHeader(State& state, int16_t x, int16_t y) {
        Coords coords = writeText(x, y, state.weatherString, Config::smallFont);
        
        return coords;
    }

    String getButtonStr(int btnIndex) {
        if (btnIndex < Config::numMainButtons) {
                return String(btnIndex+1); 
        } else {
            btnIndex -= Config::numMainButtons;
            return "^" + String(btnIndex+1);
        }
    }

    // input: top left coords of rightmost button
    void drawSpecialButtons(State& state, int16_t x, int16_t y) {
        drawIcon(x, y, refreshIconBitmap, Config::refreshDataButtonIndex);

        x = x - Config::iconSize - Config::iconSpacingX;
        drawIcon(x, y, displayIconBitmap, Config::refreshDisplayButtonIndex);

        if (state.hasNextPage) {
            x = x - Config::iconSize - Config::iconSpacingX;
            drawIcon(x, y, downArrowIconBitmap, Config::nextPageButtonIndex);
        }
    }

    void drawIcon(int16_t x, int16_t y, const uint8_t *bitmap, int btnIndex) {
        display.drawRect(x, y, Config::iconSize, Config::iconSize, GxEPD_BLACK);
        display.drawInvertedBitmap(x, y, bitmap, Config::iconSize, Config::iconSize, GxEPD_BLACK);

        if (btnIndex != -1) {
            int16_t textTopY = y + Config::iconSize + Config::iconLabelSpacing;
            writeTextMidTopAnchor(x, textTopY, Config::iconSize, getButtonStr(btnIndex), Config::tinyFont);
        }
    }

    Coords drawTaskListHeader(int16_t x, int16_t y) {
        Coords coords = writeText(x, y, "TODAY", Config::smallFont, { TextAnchorX::Middle, TextAnchorY::Top });

        return coords;
    }

    Coords drawCheckbox(int16_t leftX, int16_t topY) {
        display.drawRect(leftX, topY, Config::checkboxSize, Config::checkboxSize, GxEPD_BLACK);

        return { static_cast<int16_t>(leftX + Config::checkboxSize), static_cast<int16_t>(topY + Config::checkboxSize) };
    }
    
    void drawTaskCheckboxAsCompleted(Task& task) {
        int16_t startX = task.xStartCoord + Config::checkmarkMainOffset;
        int16_t startY = task.yStartCoord + Config::checkmarkMainOffset;

        int16_t intersectionX = task.xStartCoord + (Config::checkboxSize / 2);
        int16_t intersectionY = task.yStartCoord + (Config::checkboxSize - Config::checkmarkMainOffset);

        int16_t endX = task.xStartCoord + Config::checkboxSize + Config::checkmarkMinorOffset;
        int16_t endY = task.yStartCoord - Config::checkmarkMainOffset;

        display.drawLine(startX, startY, intersectionX, intersectionY, GxEPD_BLACK);
        display.drawLine(intersectionX, intersectionY, endX, endY, GxEPD_BLACK);
    }

    Coords drawTask(Task& task) {
        int16_t x = task.xStartCoord;
        int16_t y = task.yStartCoord;
        int16_t yBottom;

        writeText((x - Config::buttonStrCheckboxSpacingX), (y + Config::checkboxSize/2), getButtonStr(task.btnIndex), Config::tinyFont, { TextAnchorX::Right, TextAnchorY::Middle });

        Coords coords = drawCheckbox(x,y);
        x = coords.x;
        yBottom= coords.y;

        if (task.isCompleted) {
            drawTaskCheckboxAsCompleted(task);
        }

        x += Config::checkboxLabelSpacingX;
        writeTextMidLeftAnchor(x, y, Config::checkboxSize, task.label, Config::bodyFont, Config::maxTaskLabelWidth);

        x = Config::taskDetailsOffsetX;
        x = writeTextMidLeftAnchor(x, y, Config::checkboxSize, task.getTaskDetailsString(), Config::smallFont).x;
        
        return { x, yBottom };
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

    void displayUpdateTaskCompletionStatus(Task& task) {
        int16_t x, y, w, h;
        x = task.xStartCoord;
        y = task.yStartCoord - Config::checkmarkMainOffset;
        w = Config::checkboxSize + Config::checkmarkMinorOffset;
        h = Config::checkboxSize + Config::checkmarkMainOffset;
        displayPartialPage(x, y, w, h, [&]() {
            drawCheckbox(task.xStartCoord, task.yStartCoord);
            if (task.isCompleted) {
                drawTaskCheckboxAsCompleted(task);
            }
        });
    }

    void displayStateScreen(State& state) {
        displayFullPage([&]() {
            int16_t cursorX = Config::titleOffsetX;
            int16_t cursorY = Config::titleOffsetY;
            cursorY = drawHeader(state, cursorX, cursorY).y;
            
            cursorY += Config::titleSubtitleSpacing;
            cursorY = drawSubHeader(state, cursorX, cursorY).y;

            drawSpecialButtons(state, (Config::screenWidth - Config::iconOffsetRightX - Config::iconSize), Config::iconOffsetY);

            cursorX = Config::taskOffsetX + (Config::checkboxSize / 2);
            cursorY += Config::sectionSpacing;
            cursorY = drawTaskListHeader(cursorX, cursorY).y;

            cursorX = Config::taskOffsetX;
            cursorY += Config::taskRowSpacing;
            for (int i = 0; i < state.tasks.size(); i++) {
                Task& task = state.tasks[i];
                task.xStartCoord = cursorX;
                task.yStartCoord = cursorY;

                cursorY = drawTask(task).y;

                cursorY += Config::taskRowSpacing;
            }
        });
    }
}