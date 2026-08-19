#ifndef TASK_H
#define TASK_H

#include "Arduino.h"

struct Task {
    int taskId;
    int xStartCoord;
    int yStartCoord;
    String label;
    int statusCode;
    String statusLabel;

    explicit Task(int taskId, String label, int statusCode) : taskId(taskId),label(label),statusCode(statusCode) {}

};

#endif