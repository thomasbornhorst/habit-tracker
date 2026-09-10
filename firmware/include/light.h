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

    /**
     * @brief Initialize light pin
     * 
     */
    void init() {
        pinMode(pin,OUTPUT);
    }

    /**
     * @brief Update LEDs based on current state. Only changes if blinking
     * 
     */
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

    /**
     * @brief Turn on light
     * 
     */
    void turnOn() {
        digitalWrite(pin,HIGH);
        isOn = true;
    }

    /**
     * @brief Turn off light
     * 
     */
    void turnOff() {
        digitalWrite(pin,LOW);
        isOn = false;
    }

    /**
     * @brief Change light to opposite of current state. 
     * If ON, turns it OFF. If OFF, turns it ON.
     * 
     */
    void changeState() {
        if (isOn) {
            turnOff();
        } else {
            turnOn();
        }
    }

    /**
     * @brief Start a blinking sequence for a light. 
     * 
     * @param interval Interval between light state changes
     * @param totalLen Total length for light to blink. If less than or equal to 0, no time limit set on blinking.
     */
    void startBlinking(int interval, int totalLen) {
        turnOn();
        
        isBlinking = true;
        blinkInterval = interval;
        lastStateChange = millis();

        if (totalLen > 0) {
            endOfBlinking = millis() + totalLen;
        }
    }

    /**
     * @brief Start a blinking sequence for a light for an undefined amount of time
     * 
     * @param interval Interval between light state changes
     */
    void startBlinking(int interval) {
        startBlinking(interval, 0);
    }

    /**
     * @brief Start a blinking sequence for a light for an undefined amount of time with default intervals
     * 
     */
    void startBlinking() {
        startBlinking(500);
    }

    /**
     * @brief Stop light blinking sequence
     * 
     */
    void stopBlinking() {
        turnOff();
        isBlinking = false;
    }
};

#endif