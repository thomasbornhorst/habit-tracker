#include <Arduino.h>
#include <Config.h>
#include "task.h"
#include <vector>

namespace {
    unsigned long lastRefresh = millis();

    String dateString;

    String weatherString;

    std::vector<Task> tasks;
}

namespace RequestHandler {
    // Check if need to refresh data again
    bool shouldRefreshData() {
        return ((millis() - lastRefresh) > (Config::minBetweenRefreshes * 60000));
    }

    void refreshData() {
        lastRefresh = millis();
    }

    bool getState() {
        return false;
    }
}