#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include <curses.h>
#include "validator.h"

enum consoleColours { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

class Display : public QObject {

    Q_OBJECT

public:
    explicit Display(QObject *parent = nullptr);

    ~Display();
private:


    WINDOW * win;
    void initDisplay();
    void createWindow();
signals:

};

#endif // DISPLAY_H
