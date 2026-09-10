#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "state.h"
#include "ArduinoJson.h"
#include "task.h"

namespace RequestHandler {
    /**
     * @brief Check if it's time for an automated retrieval of new data
     * 
     * @return True if data should be refreshed.
     * @return False otherwise
     */
    bool shouldRefreshData();

    /**
     * @brief Get the current state from the server by running a GET request
     * 
     * @param state State that's built from GET request response
     * @return True if successful.
     * @return False otherwise
     */
    bool getState(State& state);

    /**
     * @brief Build a Task object from a JSON taskObject that's returned in a GET request response
     * 
     * @param taskObject JSON task information
     * @param task Task object built in function
     * @return True if successful.
     * @return False otherwise
     */
    bool getTaskFromJson(JsonObject taskObject, Task& task);

    /**
     * @brief Parse the CadenceType from the JSON cadence type string
     * 
     * @param cadenceTypeStr JSON cadence type string
     * @return CadenceType Parsed cadence type from string. Set to Unknown if can't parse
     */
    static CadenceType parseCadenceType(String cadenceTypeStr);

    /**
     * @brief Run GET command to Ping endpoint
     * 
     * @return True if successful.
     * @return False otherwise
     */
    bool getPing();

    /**
     * @brief Send a POST to mark the task as completed
     * 
     * @param task Task to mark as completed for today
     * @return True if successful.
     * @return False otherwise
     */
    bool sendTaskCompletion(Task& task);

    /**
     * @brief Send a PATCH to void the last completion of the task
     * 
     * @param task Task to void the last completion of
     * @return True if successful.
     * @return False otherwise
     */
    bool sendTaskEventVoid(Task& task);
}

#endif