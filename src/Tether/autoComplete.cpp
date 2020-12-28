#include "display.h"

int Display::findLongestCommand() {

    int commandLength = 0;

    foreach (auto command, Validator::getValidCommands()) {
        if (command.length() > commandLength) {
            commandLength = command.length();
        }
    }

    return commandLength;
}

void Display::turnAutoCompleteOn() {

    if(autoCompleteActive) {
        return;
    }

    autoCompleteActive   = true;
    autoCompletePosition = inputCursorPosition;
    autoCompleteString   = "";
    autoCompleteOption   = 0;
}

void Display::turnAutoCompleteOff() {

    if(!autoCompleteActive) {
        return;
    }

    autoCompleteActive = false;
}

void Display::autoCompleteOptionUp() {

    autoCompleteOption--;

    if(autoCompleteOption < 1) {
        autoCompleteOption = autoCompleteOptions;
    }
}

void Display::autoCompleteOptionDown() {

    autoCompleteOption++;

    if(autoCompleteOption > autoCompleteOptions) {
        autoCompleteOption = 1;
    }
}

void Display::autoCompleteOptionEnter() {

    // Auto complete options are offset +1
    QString command = Validator::getValidCommands().at(autoCompleteOption - 1);

    insertString(command + "] ");

    turnAutoCompleteOff();
}
