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
    QHash<int, int> normalKeyOptionMap;

    QString inputString     {""};
    int inputCursorPosition {0};


    WINDOW * win;
    void initDisplay();
    void createWindow();
    void doBackspace();
    void initSpecialKeyMap();
    void processSpecialKey(int keyCode);
    void doLeftKey();
    void doRightKey();
    void addCharacter(int ch);
    void insertCharacter(int ch);
    void processNormalKey(int ch);
    void doHomeKey();
    void doEndKey();
    void processKeys();
signals:

private:
    void initNormalKeyOptionMap();
};

#endif // DISPLAY_H
