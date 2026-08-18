#include "network.h"
#include "WiFi.h"
#include "secrets.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

bool connectToNetwork(unsigned long timeout) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long start = millis();
    while ((WiFi.status() != WL_CONNECTED) && ((millis() - start) < timeout)) {
        delay(250);
    }
    return (WiFi.status() == WL_CONNECTED);
}

bool networkIsConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

bool networkGetPing() {
    Serial.println(WiFi.status());
    if (!networkIsConnected()) {
        return false;
    }

    HTTPClient http;
    http.begin(String(BACKEND_BASE) + "/api/ping");
    //http.addHeader
    http.setTimeout(5000);
    int code = http.GET();
    bool ok = false;

    if (code == 200) {
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, http.getStream());

        if (err) {
            Serial.printf("JSON parse failed. Error: %s\n", err.c_str());
        } else {
            const char* time = doc["time"];
            ok = true;
        }
    } else {
        Serial.printf("Request failed. Code: %d\n", code);
    }

    http.end();
    return ok;
}