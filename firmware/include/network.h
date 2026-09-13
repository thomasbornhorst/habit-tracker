#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ArduinoJson.h>

namespace Network {
    /**
     * @brief Try to connect to server network w/ defined timeout
     * 
     * @param timeout Timeout for network request in ms
     * @return True if successfully connected to network.
     * @return False otherwise
     */
    bool connectToNetwork(unsigned long timeout);

    /**
     * @brief Check if device is currently connected to the server network
     * 
     * @return True if device is currently connected to the network.
     * @return False otherwise
     */
    bool isNetworkConnected();

    /**
     * @brief Send a GET request to the server with a defined apiRoute and JSON body that gets populated by response
     * 
     * @param apiRoute API route on server (e.g. api/ping)
     * @param doc JSON body document returned in response from server
     * @return True if successful.
     * @return False otherwise
     */
    bool sendGetToServer(String apiRoute, JsonDocument& doc);

    /**
     * @brief Send a POST request to the server with a defined apiRoute and JSON body
     * 
     * @param apiRoute API route on server (e.g. api/ping)
     * @param reqDoc JSON body document to send in request
     * @param resDoc JSON body document retrieved in response
     * @return True if successful.
     * @return False otherwise
     */
    bool sendPostToServer(String apiRoute, const JsonDocument& reqDoc, JsonDocument& resDoc);

    /**
     * @brief Send a PATCH request to the server with a defined apiRoute and JSON body
     * 
     * @param apiRoute API route on server (e.g. api/ping)
     * @param doc JSON body document to send in request
     * @return True if successful.
     * @return False otherwise
     */
    bool sendPatchToServer(String apiRoute, const JsonDocument& doc);
}

#endif