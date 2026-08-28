#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include "Config.h"
#include <vector>
#include "task.h"
#include "button.h"

struct State {
    String name;
    String weatherString;
    String dateString;
    unsigned long timestampUTC;
    unsigned long millisAtTimestamp;

    std::vector<Task> tasks;
    ButtonBinding buttonBindings[Config::numButtonOptions];
    bool hasNextPage = false;

    explicit State() {}

    void rebuildButtons() {
        int numTasksToShow = tasks.size();
        if (numTasksToShow > Config::maxTasksToShow) {
            numTasksToShow = Config::maxTasksToShow;
            hasNextPage = true;
        }

        int i = 0;
        for (; i < numTasksToShow; i++) {
            buttonBindings[i] = ButtonBinding(ButtonAction::ToggleTask, i);
            tasks[i].btnIndex = i;
        }
        for (; i < Config::maxTasksToShow; i++) {
            buttonBindings[i] = ButtonBinding(ButtonAction::None);
        }
        buttonBindings[Config::nextPageButtonIndex] = ButtonBinding(ButtonAction::NextPage);
        buttonBindings[Config::refreshDisplayButtonIndex] = ButtonBinding(ButtonAction::RefreshDisplay);
        buttonBindings[Config::refreshDataButtonIndex] = ButtonBinding(ButtonAction::RefreshData);
    }
};

#endif