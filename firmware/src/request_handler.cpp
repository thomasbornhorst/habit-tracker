#include <Arduino.h>
#include <Config.h>
using namespace Config;

namespace {
    unsigned long lastRefresh = millis();
}

bool getState() {
    return false;
}

// Check if need to refresh data again
bool shouldRefreshData() {
    if ((millis() - lastRefresh) > (minBetweenRefreshes * 60000)) {
        return true;
    }

    return false;
}

void refreshData() {
    lastRefresh = millis();
}