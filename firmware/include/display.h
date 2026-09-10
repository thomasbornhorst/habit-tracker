#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "state.h"

namespace Display {
    struct Coords { int16_t x, y; };

    enum class TextAnchorX { Left, Right, Middle };
    enum class TextAnchorY { Top, Middle };

    struct TextAnchors { 
        TextAnchorX textAnchorX; 
        TextAnchorY textAnchorY; 
    };

    /**
     * @brief Initialize display
     * 
     */
    void initDisplay();

    /**
     * @brief Display a full new page on the display
     * 
     * @param drawFn Callback function to be called while screen is being drawn
     */
    void displayFullPage(std::function<void()> drawFn);

    /**
     * @brief Display a new partial page on the display
     * 
     * @param x Starting x-coord of partial display window
     * @param y Starting y-coord of partial display window
     * @param w Width of partial display window
     * @param h Heigh of partial display window
     * @param drawFn Callback function to be called while screen is being drawn
     */
    void displayPartialPage(int x, int y, int w, int h, std::function<void()> drawFn);

    Coords writeTextMidLeftAnchor(int16_t leftX, int16_t topY, uint16_t rowHeight, String text, const GFXfont *font, uint16_t maxW = -1);

    Coords writeTextMidTopAnchor(int16_t leftX, int16_t topY, uint16_t rowWidth, String text, const GFXfont *font, uint16_t maxW = -1);

    /**
     * @brief Write text to the display at a specific point
     * 
     * @param targetX Target x-coord, anchored to text as defined by textAnchors
     * @param targetY Target y-coord, anchored to text as defined by textAnchors
     * @param text Text to display
     * @param font Font to use for text
     * @param textAnchors How to anchor the text. Defaults to Top Left (targetX would be leftmost point, target Y would be topmost point)
     * @param maxW Max width of text to display. If -1, no max is applied. Defaults to -1.
     * If text is longer than max width, it's cut off at the appropriate spot with a ".."
     * @return Bottom right coords of the display output 
     */
    Coords writeText(int16_t targetX, int16_t targetY, String text, const GFXfont *font, TextAnchors textAnchors = {TextAnchorX::Left,TextAnchorY::Top}, uint16_t maxW = -1);

    /**
     * @brief Draw header for given state
     * 
     * @param state Current state
     * @param x Starting x-coord
     * @param y Starting y-coord
     * @return Bottom right coords of the display output  
     */
    Coords drawHeader(State& state, int16_t x, int16_t y);

    /**
     * @brief Draw the sub-header, for extra information below the main header
     * 
     * @param state Current state
     * @param x Starting x-coord
     * @param y Starting y-coord
     * @return Bottom right coords of the display output  
     */
    Coords drawSubHeader(State& state, int16_t x, int16_t y);

    /**
     * @brief Get the the string label for a given button index
     * 
     * @param btnIndex Button index to retrieve the label for
     * @return String 
     */
    String getButtonStr(int btnIndex);

    /**
     * @brief Draw the non-task special buttons like "refresh data"
     * 
     * @param state Current state
     * @param x Starting x-coord of rightmost button
     * @param y Starting y-coord of rightmost button
     */
    void drawSpecialButtons(State& state, int16_t x, int16_t y);

    /**
     * @brief Draw an icon for a button
     * 
     * @param x Starting x-coord
     * @param y Starting y-coord
     * @param bitmap Bitmap for the icon
     * @param btnIndex If not -1, adds a button label below the button icon
     */
    void drawIcon(int16_t x, int16_t y, const uint8_t *bitmap, int btnIndex = -1);

    /**
     * @brief Draw header for the list of tasks
     * 
     * @param x Starting x-coord
     * @param y Starting y-coord 
     * @return Bottom right coords of the display output 
     */
    Coords drawTaskListHeader(int16_t x, int16_t y);

    /**
     * @brief Draw checkbox at given location
     * 
     * @param leftX Leftmost x-coord of checkbox
     * @param topY Topmost y-coord of checkbox
     * @return Bottom right coords of the display output 
     */
    Coords drawCheckbox(int16_t leftX, int16_t topY);
    
    void drawTaskCheckboxAsCompleted(Task& task);

    /**
     * @brief Draw task, including the button label, checkbox, main label, and details
     * 
     * @param task Task to be drawn
     * @return Bottom right coords of the display output 
     */
    Coords drawTask(Task& task);

    /**
     * @brief Displays the screen shown at startup
     * 
     */
    void displayStartupScreen();

    /**
     * @brief Displays the screen shown when data could not be retrieved
     * 
     */
    void displayNoContentErrorScreen();

    /**
     * @brief Updates a task's checkbox after its status has changed
     * 
     * @param task Task that has been updated
     */
    void displayUpdateTaskCompletionStatus(Task& task);

    /**
     * @brief Display full screen for a given state
     * 
     * @param state Current state
     */
    void displayStateScreen(State& state);
}

#endif