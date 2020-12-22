#include "program.h"
#include "display.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Program p;

    Display d;

    return a.exec();
}
