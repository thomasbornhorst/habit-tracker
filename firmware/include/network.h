#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ArduinoJson.h>

namespace Network {
    bool connectToNetwork(unsigned long timeout);

    bool isNetworkConnected();

    bool sendGetToServer(String apiRoute, JsonDocument& doc);

    bool sendPostToServer(String apiRoute, const JsonDocument& doc);

    bool sendPatchToServer(String apiRoute, const JsonDocument& doc);
}

#endif