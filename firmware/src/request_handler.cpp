#include <Arduino.h>
#include <Config.h>
#include "task.h"
#include <vector>
#include "network.h"
#include "ArduinoJson.h"

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
        JsonDocument doc;
        bool success = Network::sendGetToServer("/api/state", doc);
        if (!success) {
            return false;
        }

        const char* name = doc["name"];
        Serial.println(name);

        JsonArray tasks = doc["tasks"];
        for (JsonObject task : tasks) {
            int id = task["id"];
            const char* taskLabel = task["label"];
            Serial.println(id);
            Serial.println(taskLabel);
        }

        return true;
    }

    bool getPing() {
        JsonDocument doc;
        bool success = Network::sendGetToServer("/api/ping", doc);
        if (!success) {
            return false;
        }

        const char* time = doc["time"];
        Serial.println(time);
        return true;
    }
}