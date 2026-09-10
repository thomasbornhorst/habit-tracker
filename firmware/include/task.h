#ifndef TASK_H
#define TASK_H

#include "Arduino.h"

enum class CadenceType { Daily, Rolling, Decay, Weekly, Unknown };

struct Task {
    // Core task attributes
    int id;
    String label;
    CadenceType cadenceType;
    int cadenceVal;
    int eventId = -1;

    // Display
    int xStartCoord;
    int yStartCoord;
    int btnIndex;
    
    // Status
    int statusCode;
    String statusLabel;
    bool isCompleted;
    String lastCompDateStr; // for non-weekly
    int numCompsForWeek; // for weekly

    /**
     * @brief Get string for the details of the task
     * 
     * @return String of details for the task (e.g. "Last: 11/23")
     */
    String getTaskDetailsString() const {
        if (cadenceType == CadenceType::Daily) {
            return getLastCompletedString();
        } else if (cadenceType == CadenceType::Rolling) {
            return getLastCompletedString(); // + " | Due: " + "unk"; // TODO: Implement showing the due date here
        } else if (cadenceType == CadenceType::Decay) {
            return getLastCompletedString(); // TODO: Implement showing the decay due date here or something about its status
        } else if (cadenceType == CadenceType::Weekly) {
            return ("Week: " + String(numCompsForWeek) + "/" + String(cadenceVal));
        }

        return "";
    }

    /**
     * @brief Get string for the details of the last completed date of the task 
     * 
     * @return String of last completed date (e.g. "Last 11/23")
     */
    String getLastCompletedString() const {
        if (lastCompDateStr == "null") {
            return "Last: N/A";
        }
        return ("Last: " + lastCompDateStr);
    }
};

#endif