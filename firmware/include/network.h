#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ArduinoJson.h>

namespace Network {
    bool connectToNetwork(unsigned long timeout);

    bool isNetworkConnected();

    bool sendGetToServer(String apiRoute, JsonDocument& doc);
}

#endif