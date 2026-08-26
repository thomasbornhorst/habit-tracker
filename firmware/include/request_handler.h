#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "state.h"
#include "ArduinoJson.h"
#include "task.h"

namespace RequestHandler {
    // Check if need to refresh data again
    bool shouldRefreshData();

    bool getState(State& state);

    bool getTaskFromJson(JsonObject taskObject, Task& task);

    static CadenceType parseCadenceType(String cadenceTypeStr);

    bool getPing();

    bool sendTaskCompletion(Task& task);

    bool sendTaskEventVoid(Task& task);
}

#endif