#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

struct Light {
    uint8_t pin;

    explicit Light(uint8_t pin) : pin(pin) {}

    void init() {
        pinMode(pin,OUTPUT);
    }

    void turnOn() {
        digitalWrite(pin,HIGH);
    }

    void turnOff() {
        digitalWrite(pin,LOW);
    }
};

#endif