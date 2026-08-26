#include <Arduino.h>
#include "display.h"
#include "io_control.h"
#include "request_handler.h"
#include "network.h"
#include "state.h"

State mainState;

void flashGreenStatus() {
    IO::greenLED.turnOn();
    delay(100);
    IO::greenLED.turnOff();
}

void getNewState() {
    IO::redLED.turnOn();
    if (RequestHandler::getState(mainState)) {
        flashGreenStatus();
        mainState.rebuildButtons();
        Display::displayStateScreen(mainState);
    };
    IO::redLED.turnOff();
}

void setup() {
    Serial.begin(115200);

    Display::initDisplay();
    IO::initButtonsAndLEDs();
    IO::redLED.turnOn();

    Display::displayStartupScreen();
    
    if (Network::connectToNetwork(5000)) {
        flashGreenStatus();
    }

    getNewState();

    IO::redLED.turnOff();
}

void handleButtonPress(int buttonPressIndex) {
    IO::redLED.turnOn();
    ButtonBinding btnBinding = mainState.buttonBindings[buttonPressIndex];
    switch (btnBinding.action) {
        case ButtonAction::ToggleTask: {
            Task& task = mainState.tasks[btnBinding.taskIndex];
            task.isCompleted = !task.isCompleted;
            Display::displayUpdateTaskCompletionStatus(task);

            // TODO: Queue up these completions instead of sending them right away?
            if (task.isCompleted) {
                if (RequestHandler::sendTaskCompletion(task)) {
                    flashGreenStatus();
                }
                // TODO: Deal with failed task update?
            } else if (task.eventId > 0) {
                if (RequestHandler::sendTaskEventVoid(task)) {
                    flashGreenStatus();
                }
                // TODO: Deal with failed task update?
            }

            break;
        }
        case ButtonAction::NextPage:
            break;
        case ButtonAction::RefreshDisplay:
            Display::displayStateScreen(mainState);
            break;
        case ButtonAction::RefreshData:
            getNewState();
            break;
        default: break;
    }
    IO::redLED.turnOff();
}

void loop() {
    IO::updateButtons();
    IO::updateLEDs();

    // If queued update => send it if it's time ; DON'T REFRESH FROM SERVER IF UPDATE QUEUED TO BE SENT

    if (RequestHandler::shouldRefreshData()) {
        getNewState();
    }

    if (IO::buttonPressedIndex != -1) {
        handleButtonPress(IO::buttonPressedIndex);
    }
}