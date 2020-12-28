#include "display.h"

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <utility>

Display::Display(QObject *parent) : QObject(parent) {


    initDisplay();

    initSpecialKeyMap();

    initAutocompleteKeyMap();

    initNormalKeyOptionMap();

    processKeys();
}

void Display::processKeys() {

    while (true) {

        // Block for new key
        int ch = getch();

        // Differentiate between special and normal keys
        if(ch >= KEY_MIN && ch <= KEY_MAX) {

            processSpecialKey(ch);
        } else if (ch == 10) {

            processSpecialKey(KEY_ENTER);
        } else {

            processNormalKey(ch);
        }

        // Validate the input string
        inputStringIsValid = Validator::validateLine(inputString);

        // Redraw entire input string
        redrawInputString();

        // Do autocompletion
        if(autoCompleteActive) {
            autoComplete();
        }

        // Refresh the window for changes to take effect
        wrefresh(win);
    }
}

void Display::autoComplete() {

    // Set autoComplete below open bracket
    wmove(win, getCurrentRow() + 1, autoCompletePosition - 1);

    // Show valid commands

    // Find the longest
    int commandLength = findLongestCommand();

    int i = 1;
    foreach (auto command, Validator::getValidCommands()) {

        if (i > maxAutoCompleteOptions) {
            break;
        }

        wmove(win, getCurrentRow() + i, autoCompletePosition - 1);

        // Get the description string
        QString descriptionString = Validator::getValidCommandDescription(command);

        QChar pad(' ');

        // Prepend a space
        QString paddedCommand = command.prepend(pad);

        // Now pad the string so all are the same length
        paddedCommand = paddedCommand.leftJustified(commandLength + 2, ' ');

        // Place the command string
        autoCompleteOption == i ? wattron(win, COLOR_PAIR(4)) : wattron(win, COLOR_PAIR(3));
        waddstr(win, paddedCommand.toLocal8Bit().data());

        // Calculate space left for description
        int descriptionLength = getWindowWidth() - autoCompletePosition - commandLength;

        wattron(win, COLOR_PAIR(2));

        // Add some padding to the beginning
        descriptionString.prepend(pad);

        // Place the description string
        if(descriptionString.length() > descriptionLength) {
            int difference = descriptionLength - descriptionString.length();
            descriptionString.remove(descriptionString.length() - difference - 1, difference);
        }

        waddstr(win, descriptionString.toLocal8Bit().data());

        i++;
    }

    wattron(win, COLOR_PAIR(1));

    // Return window cursor
    wmove(win, getCurrentRow(), getCurrentColumn());
}

void Display::processNormalKey(int ch) {

    insertCharacter(ch);

    if(autoCompleteActive) {
        autoCompleteString.append(QChar(ch).toUpper());
    }

    if(ch == '[') {
        turnAutoCompleteOn();
    } else if (ch == ']') {
        turnAutoCompleteOff();
    }
}

void Display::insertCharacter(int ch) {

    // Add char to input string
    inputString.insert(inputCursorPosition,
                       autoCompleteActive ? QChar(ch).toUpper() : QChar(ch));

    // Move window cursor right
    doRightKey();
}

void Display::insertString(QString string) {

    inputString.insert(inputCursorPosition, string);

    inputCursorPosition += string.length();
}

void Display::redrawInputString() {

    // Clear window
    werase(win);

    // Set window cursor to start position
    wmove(win, 0, 0);

    char ch = ' ';
    chtype option = A_NORMAL;


    // TODO: If string is invalid, change colour

    // Write the string
    for (int i = 0; i < inputString.length(); i ++) {

        // Get the char from the input string
        ch = inputString.at(i).toLatin1();

        // Get the char options
        normalKeyOptionMap.contains(ch) ?  option = normalKeyOptionMap.value(ch) : option = A_NORMAL;

        // Add the char, with options, to the window
        waddch(win, ch | option);
    }

    // Set the cursor back to the users position
    wmove(win, getCurrentRow(), getCurrentColumn());
}

int Display::getWindowWidth() {

    // Get the window dimensions
    int h, w;
    getmaxyx(win, h, w);

    return w;
}

int Display::getWindowHeight() {

    // Get the window dimensions
    int h, w;
    getmaxyx(win, h, w);

    return h;
}

int Display::getCurrentRow() {

    return inputCursorPosition / getWindowWidth();
}

int Display::getCurrentColumn() {

    return inputCursorPosition - (getCurrentRow()  * getWindowWidth());
}

int Display::getNumberOfRowsInString() {

    return (inputString.length() / getWindowWidth()) + 1;
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
    int windowWidth = getWindowWidth();

    // Add the difference between the window width and the window cursor position.
    // If this is beyond the input string length, subtract and add the difference
    // as the new position.
    int distance = windowWidth - x - 1;
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

            y--;
            x = getWindowWidth() - 1;
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

        // If the cursor is at the end of the line, go down one
        if(x == (getWindowWidth() - 1)) {
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

    // Remove char from inputString
    inputString.remove(inputCursorPosition, 1);
}

void Display::doDeleteKey() {

    // Remove char from inputString
    inputString.remove(inputCursorPosition, 1);
}

Display::~Display() {
    // Restore terminal
    endwin();
}

void Display::processSpecialKey(int keyCode) {

    if(autoCompleteActive && autocompleteKeyMap.contains(keyCode)) {

        (this->*autocompleteKeyMap.value(keyCode))();

    } else if (specialKeyMap.contains(keyCode)) {

        (this->*specialKeyMap.value(keyCode))();
    }
}

void Display::initNormalKeyOptionMap() {

    normalKeyOptionMap['['] = A_BOLD | COLOR_PAIR(1);
    normalKeyOptionMap[']'] = A_BOLD | COLOR_PAIR(1);
    normalKeyOptionMap['x'] = A_BOLD | A_UNDERLINE ;
}

void Display::initAutocompleteKeyMap() {

    autocompleteKeyMap[KEY_UP]    = &Display::autoCompleteOptionUp;
    autocompleteKeyMap[KEY_DOWN]  = &Display::autoCompleteOptionDown;
    autocompleteKeyMap[KEY_ENTER] = &Display::autoCompleteOptionEnter;
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
    int rowSpan    = getNumberOfRowsInString();
    int currentRow = getCurrentRow();

    if(currentRow < rowSpan - 1) {

        // Get cursor current position
        int y;
        int x;
        getyx(win, y, x);

        // Check if the next row below is the last row
        if(currentRow + 1 == (rowSpan - 1)) {

            // Subtract the rows before the final row to get the
            // last posible cursor position
            int lastRowPosition = inputString.length() - ((rowSpan - 1) * getWindowWidth());

            // Now check if the position asked by the user (the position directly below
            // the cursor) is available
            if(lastRowPosition >= x) {

                // The position is available, we can move to it
                wmove(win, y + 1, x);

                //
                inputCursorPosition += getWindowWidth();

            } else {

                // The position is not available, move to the last possible position
                wmove(win, y + 1, lastRowPosition);

                //
                inputCursorPosition = inputString.length();
            }

        } else {

            // Just go down one
            wmove(win, y + 1, x);

            //
            inputCursorPosition += getWindowWidth();
        }
    }
}

void Display::doEnterKey() {

}

void Display::initSpecialKeyMap() {

    specialKeyMap[KEY_BACKSPACE]    = &Display::doBackspace;
    specialKeyMap[KEY_LEFT]         = &Display::doLeftKey;
    specialKeyMap[KEY_RIGHT]        = &Display::doRightKey;
    specialKeyMap[KEY_HOME]         = &Display::doHomeKey;
    specialKeyMap[KEY_END]          = &Display::doEndKey;
    specialKeyMap[KEY_UP]           = &Display::doUpKey;
    specialKeyMap[KEY_DOWN]         = &Display::doDownKey;
    specialKeyMap[KEY_DC]           = &Display::doDeleteKey;
    specialKeyMap[KEY_ENTER]        = &Display::doEnterKey;
}

void Display::initDisplay() {

    initscr(); // init ncurses

    noecho(); // Dont echo user input
    cbreak(); // Dont buffer user input
    keypad(stdscr, TRUE); // Capture special keys

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    createWindow();
}

void Display::createWindow() {

    int h, w;
       getmaxyx(stdscr, h, w);

    win = newwin(h, w, 0, 0);

}
