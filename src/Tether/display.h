#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include <QHash>

#include <curses.h>

#include "validator.h"

enum consoleColours { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

class Display : public QObject {

    Q_OBJECT

public:
    explicit Display(QObject *parent = nullptr);

    ~Display();
private:

    QHash<int, void (Display::*)(void)> specialKeyMap;
    QHash<int, void (Display::*)(void)> autocompleteKeyMap;
    QHash<char, chtype> normalKeyOptionMap;

    QString inputString     {""};
    int inputCursorPosition {0};

    WINDOW * win;

    bool wasValid             {false};
    bool autoCompleteActive   {false};
    int  autoCompletePosition {0};
    int  autoCompleteOption   {0};

    QString autoCompleteString {""};

    static const int autoCompleteOptions {6};

    void initDisplay();
    void createWindow();
    void doBackspace();
    void initSpecialKeyMap();
    void initNormalKeyOptionMap();
    void processSpecialKey(int keyCode);

    void insertCharacter(int ch);
    void processNormalKey(int ch);
    void doHomeKey();
    void doEndKey();
    void processKeys();

    void setInputStringValidation(bool valid);

    void doUpKey();
    void doDownKey();
    void doLeftKey();
    void doRightKey();
    int getNumberOfRowsInString();
    int getCurrentRow();
    int getWindowHeight();
    int getWindowWidth();
    int getCurrentColumn();
    void redrawInputString();
    void doDeleteKey();
    void doEnterKey();
    void autoComplete();
    int findLongestCommand();
    void turnAutoCompleteOn();
    void turnAutoCompleteOff();
    void autoCompleteOptionUp();
    void autoCompleteOptionDown();
    void initAutocompleteKeyMap();
    void autoCompleteOptionEnter();
    void insertString(QString string);
};

#endif // DISPLAY_H
