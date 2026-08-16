#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

struct Light {
    uint8_t pin;
    bool isOn = false;

    bool isBlinking = false;
    int blinkInterval = 500;
    unsigned long lastStateChange = 0;
    unsigned long endOfBlinking = 0;

    explicit Light(uint8_t pin) : pin(pin) {}

    void init() {
        pinMode(pin,OUTPUT);
    }

    void update() {
        if (isBlinking) {
            unsigned long currentTime = millis();

            if (endOfBlinking > 0 && currentTime > endOfBlinking) {
                stopBlinking();
            }
            else if ((currentTime - lastStateChange) > blinkInterval) {
                changeState();
                lastStateChange = millis();
            }
        }
    }

    void turnOn() {
        digitalWrite(pin,HIGH);
        isOn = true;
    }

    void turnOff() {
        digitalWrite(pin,LOW);
        isOn = false;
    }

    void changeState() {
        if (isOn) {
            turnOff();
        } else {
            turnOn();
        }
    }

    void startBlinking(int interval, int totalLen) {
        turnOn();
        
        isBlinking = true;
        blinkInterval = interval;
        lastStateChange = millis();

        if (totalLen > 0) {
            endOfBlinking = millis() + totalLen;
        }
    }

    void startBlinking(int interval) {
        startBlinking(interval, 0);
    }

    void startBlinking() {
        startBlinking(500);
    }

    void stopBlinking() {
        turnOff();
        isBlinking = false;
    }
};

#endif