#include <Arduino.h>
#include "display.h"
#include "io_control.h"
#include "request_handler.h"
#include "network.h"
#include "state.h"

State mainState;

/**
 * @brief Quickly flash Green LED to notify of a success
 * 
 */
void flashGreenStatus() {
    IO::greenLED.turnOn();
    delay(100);
    IO::greenLED.turnOff();
}

/**
 * @brief Retrieve the current state from the network.
 * If successful, rebuild button mappings and re-display the new state
 * 
 * @param showErrorOnFailure If true, show error screen if failed to get new state. Defaults to false
 */
void getNewState(bool showErrorOnFailure = false) {
    IO::redLED.turnOn();
    if (RequestHandler::getState(mainState)) {
        flashGreenStatus();
        mainState.rebuildButtons();
        Display::displayStateScreen(mainState);
    } else if (showErrorOnFailure) {
        mainState.rebuildButtons();
        Display::displayNoContentErrorScreen();
    }
    IO::redLED.turnOff();
}

/**
 * @brief Main setup function run at micro-controller startup
 * 
 */
void setup() {
    Serial.begin(115200);

    Display::initDisplay();
    IO::initButtonsAndLEDs();
    IO::redLED.turnOn();

    Display::displayStartupScreen();
    
    if (Network::connectToNetwork(5000)) {
        flashGreenStatus();
    }

    getNewState(true);

    IO::redLED.turnOff();
}

/**
 * @brief Given the index of a button press, runs the appropriate actions
 * 
 * @param buttonPressIndex Array index of the button that was pressed
 */
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
            getNewState(!mainState.dateString);
            break;
        default: break;
    }
    IO::redLED.turnOff();
}

/**
 * @brief Main loop function for micro-controller
 * 
 */
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