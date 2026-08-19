#ifndef DISPLAY_H
#define DISPLAY_H

namespace Display {
    void initDisplay();

    void displayStartupScreen();

    void displayPartialPage(int x, int y, int w, int h, std::function<void()> drawFn);

    void displayStartupScreen();
}

#endif