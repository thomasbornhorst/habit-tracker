#include <Arduino.h>
#include <Config.h>
#include "task.h"
#include <vector>
#include "network.h"
#include "ArduinoJson.h"
#include "state.h"
#include "task.h"
#include "request_handler.h"

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

    // Send GET to server for state, populate passed-in State object with the response 
    bool getState(State& state) {
        lastRefresh = millis();

        JsonDocument doc;
        bool success = Network::sendGetToServer("/api/state", doc);
        if (!success) {
            return false;
        }

        state = State();
        state.name = doc["name"].as<String>();
        state.dateString = doc["date"].as<String>();
        state.weatherString = doc["weather"].as<String>();
        state.timestampUTC = doc["timestampUTC"];
        state.millisAtTimestamp = millis();

        JsonArray tasks = doc["tasks"];
        for (JsonObject taskObj : tasks) {
            Task newTask = Task();
            if (getTaskFromJson(taskObj, newTask)) {
                state.tasks.push_back(newTask);
            }
        }

        return true;
    }

    bool getTaskFromJson(JsonObject taskObject, Task& task) {
        task.id = taskObject["id"];
        task.label = taskObject["label"].as<String>();
        task.cadenceType = parseCadenceType(taskObject["cadenceType"].as<String>());
        task.cadenceVal = taskObject["cadenceValue"];
        task.statusCode = taskObject["status"];
        task.eventId = taskObject["eventId"] | -1;
        task.isCompleted = (task.eventId > 0);
        task.lastCompDateStr = taskObject["lastCompletionDateStr"].as<String>();
        task.numCompsForWeek = taskObject["numCompletionsThisWeek"];

        if (task.id <= 0) {
            Serial.println("Invalid task id"); //TODO include taskID
            return false;
        } else if (task.cadenceType == CadenceType::Unknown) {
            Serial.println("Unknown cadence type (" + taskObject["cadenceType"].as<String>() + ") for task id: " + String(task.id));
            return false;
        }

        return true;
    }

    static CadenceType parseCadenceType(String cadenceTypeStr) {
        if (cadenceTypeStr.equalsIgnoreCase("Daily")) {
            return CadenceType::Daily;
        } else if (cadenceTypeStr.equalsIgnoreCase("Rolling")) {
            return CadenceType::Rolling;
        } else if (cadenceTypeStr.equalsIgnoreCase("Decay")) {
            return CadenceType::Decay;
        } else if (cadenceTypeStr.equalsIgnoreCase("Weekly_Quota")) {
            return CadenceType::Weekly;
        }

        return CadenceType::Unknown;
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

    bool sendTaskCompletion(Task& task) {
        JsonDocument doc;
        doc["taskId"] = task.id;
        doc["source"] = "board-display";
        return Network::sendPostToServer("/api/events", doc);
    }

    bool sendTaskEventVoid(Task& task) {
        JsonDocument doc;
        doc["taskId"] = task.id;
        doc["source"] = "board-display";
        return Network::sendPatchToServer("/api/events/:" + String(task.eventId) + "/void", doc);
    }
}