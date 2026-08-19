#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Config.h>
#include "display.h"
#include <functional>

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
}