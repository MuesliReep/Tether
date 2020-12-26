#include "display.h"

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <utility>

Display::Display(QObject *parent) : QObject(parent) {


    initDisplay();

    initSpecialKeyMap();

    initNormalKeyOptionMap();

    processKeys();
}

void Display::processKeys() {

    while (true) {

        // Block for new key
        int ch = getch();

        // Differentiate between special and normal keys
        if(ch >= KEY_MIN && ch) {

            processSpecialKey(ch);
        } else {

            processNormalKey(ch);
        }

        // Validate the input string
        if(Validator::validateLine(inputString)) {

            if(!wasValid) {
                setInputStringValidation(true);
                wasValid = true;
            }

        } else {

            if(wasValid) {
                setInputStringValidation(false);
                wasValid = false;
            }
        }

        // Refresh the window for it to take effect
        wrefresh(win);
    }
}

void Display::setInputStringValidation(bool valid) {

    if(valid) {
        // Set to green
    } else {
        // Set to red
    }
}

void Display::processNormalKey(int ch) {

    // Check if the character needs to be inserted or just placed at the end
    if (inputCursorPosition != inputString.length()) {
        insertCharacter(ch);
    } else {
        addCharacter(ch);
    }
}

void Display::insertCharacter(int ch) {

    // Insert char
    winsch(win, ch);

    // Add char to input string
    inputString.insert(inputCursorPosition, QChar(ch));

    // Check if the string needs to be wrapped, because winsch()
    // does not do this by itself

    // Get the window dimensions
    int h, w;
    getmaxyx(stdscr, h, w);

    if (inputString.length() > getWindowWidth()) {

        // Get cursor current position
        int userY;
        int userX;
        getyx(win, userY, userX);

        // How many rows does the input string span?
        int rowSpan    = getNumberOfRowsInString();
        int currentRow = getCurrentRow();

        for (int i = 0; i < rowSpan; i++) {
            wmove(win, userY + i, 0);
            QByteArray rowByteArray = inputString.mid(w * (currentRow + i), w).toLocal8Bit();
            const char *rowString = rowByteArray.data();
            waddstr(win, rowString);
        }

        // Finally set the window cursor to the users position
        wmove(win, userY, userX);
    }

    // Move cursor right
    doRightKey();
}

int Display::getWindowWidth() {

    // Get the window dimensions
    int h, w;
    getmaxyx(stdscr, h, w);

    return w;
}

int Display::getWindowHeight() {

    // Get the window dimensions
    int h, w;
    getmaxyx(stdscr, h, w);

    return h;
}

int Display::getCurrentRow() {

    return inputCursorPosition / getWindowWidth();
}

int Display::getNumberOfRowsInString() {

    return (inputString.length() / getWindowWidth()) + 1;
}

void Display::addCharacter(int ch) {

    //
    chtype option = A_NORMAL;
    if (normalKeyOptionMap.contains(ch)) {
        option = normalKeyOptionMap.value(ch); }

    // Add char
    waddch(win, ch | option);
//    waddch(win, 'X' | A_UNDERLINE | A_BOLD | COLOR_PAIR(3));

    // Add char to input string
    inputString.insert(inputCursorPosition, QChar(ch));
    inputCursorPosition++;
}

void Display::doHomeKey() {

    // Get cursor current position
    int y;
    int x;
    getyx(win, y, x);

    // Subtract the window cursor position from the input cursor position to
    // get the correct input cursor position when multiple lines are used
    inputCursorPosition -= x;

    // Next, just set the position to zero for the most left position
    x = 0;

    //
    wmove(win, y, x);
}

void Display::doEndKey() {

    // Get cursor current position
    int y;
    int x;
    getyx(win, y, x);

    // Get the window dimensions
    int h, w;
    getmaxyx(stdscr, h, w);

    // Add the difference between the window width and the window cursor position.
    // If this is beyond the input string length, subtract and add the difference
    // as the new position.
    int distance = w - x - 1;
    if ((inputCursorPosition + distance) <= inputString.length()) {

        //
        x += distance;
        inputCursorPosition += distance;
    } else {

        //
        //x = (inputCursorPosition + distance) - inputString.length();
        x += inputString.length() - inputCursorPosition;
        inputCursorPosition = inputString.length();
    }

    //
    wmove(win, y, x);
}

void Display::doLeftKey() {

    // Get cursor current position
    int y;
    int x;
    getyx(win, y, x);

    // TODO: if inputCursorPosition == 0, do nothing.
    if(inputCursorPosition > 0) {

        // If cursor is at left most position and a row above it is available,
        // move it to the row above, right most position
        if(x == 0 && y > 0) {

            int h, w;
            getmaxyx(stdscr, h, w);

            y--;
            x = w - 1;
        } else {
            x--; // Just go left
        }

        wmove(win, y, x);

        inputCursorPosition--;
    }
}

void Display::doRightKey() {

    // Get cursor current position
    int y;
    int x;
    getyx(win, y, x);

    // Only move right if we are not at the end of the string
    if (inputCursorPosition < inputString.length()) {

        // Get height and width of screen
        int h, w;
        getmaxyx(stdscr, h, w);

        // If the cursor is at the end of the line, go down one
        if(x == (w - 1)) {
            x = 0;
            y++;
        } else { // Just go right one position
            x++;
        }

        wmove(win, y, x);

        inputCursorPosition++;
    }
}

void Display::doBackspace() {

    // Place cursor one place to the left
    doLeftKey();

    // Remove char
    wdelch(win);

    // Remove char from inputString
    inputString.remove(inputCursorPosition, 1);

    // TODO: check if the input string is wrapped to another line
    // if so, pull it back.
}

Display::~Display() {
    // Restore terminal
    endwin();
}

void Display::processSpecialKey(int keyCode) {

    if (specialKeyMap.contains(keyCode)) {
        (this->*specialKeyMap.value(keyCode))();
    }
}

void Display::initNormalKeyOptionMap() {

    normalKeyOptionMap['['] = A_BOLD | COLOR_PAIR(1);
    normalKeyOptionMap[']'] = A_BOLD | COLOR_PAIR(1);
    normalKeyOptionMap['x'] = A_BOLD | A_UNDERLINE ;
}

void Display::doUpKey() {

    // Check if there is another row above
    int currentRow = getCurrentRow();
    if(currentRow > 0) {

        // Get cursor current position
        int y;
        int x;
        getyx(win, y, x);

        // Move it up by one
        wmove(win, y - 1, x);

        //
        inputCursorPosition -= getWindowWidth();
    }
}

void Display::doDownKey() {

    // Check if there is another row below
    int rowSpan = getNumberOfRowsInString();
    int currentRow = getCurrentRow();
    if(currentRow < rowSpan) {

    }
}

void Display::initSpecialKeyMap() {

    specialKeyMap[KEY_BACKSPACE]    = &Display::doBackspace;
    specialKeyMap[KEY_LEFT]         = &Display::doLeftKey;
    specialKeyMap[KEY_RIGHT]        = &Display::doRightKey;
    specialKeyMap[KEY_HOME]         = &Display::doHomeKey;
    specialKeyMap[KEY_END]          = &Display::doEndKey;
    specialKeyMap[KEY_UP]           = &Display::doUpKey;
    specialKeyMap[KEY_DOWN]         = &Display::doDownKey;
}

void Display::initDisplay() {

    initscr(); // init ncurses

    noecho(); // Dont echo user input
    cbreak(); // Dont buffer user input
    keypad(stdscr, TRUE); // Capture special keys

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);

    createWindow();
}

void Display::createWindow() {

    int h, w;
       getmaxyx(stdscr, h, w);

    win = newwin(h, w, 0, 0);

}
