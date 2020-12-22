#include "display.h"




#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <utility>



Display::Display(QObject *parent) : QObject(parent) {


    initDisplay();

    while (true) {
        int ch = getch();
//        std::cout << ch;
        waddch(win, ch);
        wrefresh(win);
    }
}

Display::~Display() {
    // Restore terminal
    endwin();
}

void Display::initDisplay() {

    initscr(); // init ncurses

    noecho(); // Dont echo user input
    cbreak(); // Dont buffer user input
    keypad(stdscr, TRUE); // Capture special keys

    createWindow();
}

void Display::createWindow() {

    int h, w;
       getmaxyx(stdscr, h, w);

    win = newwin(h, w, 0, 0);


}
