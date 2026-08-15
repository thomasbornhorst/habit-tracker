#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Config.h>
#include "display.h"
using namespace Config;

namespace {
    GxEPD2_BW<GxEPD2_DRIVER_CLASS, GxEPD2_DRIVER_CLASS::HEIGHT> display(
        GxEPD2_DRIVER_CLASS(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_BUSY_PIN)
    );
}

void initDisplay() {
    display.init(115200);
    display.setRotation(0);
    display.setTextColor(GxEPD_BLACK);
}

void displayStartupScreen() {
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);

        display.setFont(titleFont);
        display.setCursor(50, 100);
        display.print("WELCOME!");
        
        display.setFont(headerFont);
        display.setCursor(50, 200);
        display.print("Loading content");
    } while (display.nextPage());

    display.hibernate();
}