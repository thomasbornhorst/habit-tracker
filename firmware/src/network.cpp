#include "network.h"
#include "WiFi.h"
#include "secrets.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

namespace Network {
    bool connectToNetwork(unsigned long timeout) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        unsigned long start = millis();
        while ((WiFi.status() != WL_CONNECTED) && ((millis() - start) < timeout)) {
            delay(250);
        }
        return (WiFi.status() == WL_CONNECTED);
    }

    bool isNetworkConnected() {
        return (WiFi.status() == WL_CONNECTED);
    }

    bool sendGetToServer(String apiRoute, JsonDocument& doc) {
        if (!isNetworkConnected()) {
            return false;
        }

        HTTPClient http;
        http.begin(String(BACKEND_BASE) + apiRoute);
        //http.addHeader
        http.setTimeout(5000);

        bool ok = false;
        int code = http.GET();
        if (code == 200) {
            DeserializationError err = deserializeJson(doc, http.getStream());

            if (err) {
                Serial.printf("JSON parse failed. Error: %s\n", err.c_str());
            } else {
                ok = true;
            }
        } else {
            Serial.printf("Request failed. Code: %d\n", code);
        }

        http.end();
        return ok;
    }
}
