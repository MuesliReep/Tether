#include "program.h"

#include <QRegularExpression>
#include <QTextStream>

Program::Program(QObject *parent) : QObject(parent), notifier(STDIN_FILENO, QSocketNotifier::Read), inStream(stdin) {




//    connect(&notifier, &QSocketNotifier::activated, this, &Program::notified);

    connect(inStream.device(), &QIODevice::readyRead, this, &Program::notified);

    createInputMap();

    qDebug("Test");
    parse("test");
}

void Program::createInputMap() {

    inputMap["NEW"]         = &Program::parseNew;
    inputMap["TAG"]         = &Program::parseTag;
    inputMap["DELEGATE"]    = &Program::parseDelegate;
    inputMap["TITLE"]       = &Program::parseTitle;
}

void Program::notified() {

    qDebug("received");
}

void Program::parse(QString line) {

    line = "[NEW] aaa aaa [TITLE]bbb [DELEGATE] ccc";


//    QList<QString> duplicateCheck;



    QRegularExpression commandInputEx("(\\[\\w+\\]( |)\\w+)");
    QRegularExpression commandEx("(\\[\\w+\\])");

    QRegularExpressionMatchIterator matchIterator = commandInputEx.globalMatch(line);

    while(matchIterator.hasNext()) {

        QRegularExpressionMatch match = matchIterator.next();
        QString word = match.captured(1);
        qInfo(word.toUtf8());

        QRegularExpressionMatch match2 = commandEx.match(word);

        if(match2.hasMatch()) {

            //
            QString command = match2.captured(0);

            // Remove brackets
            command.remove('[');
            command.remove(']');

            // Parse it
            inputMap.contains(command) ? (this->*inputMap.value(command))("aaa")  : qInfo("Invalid Command!");
        }else {
            qInfo("No command match!");
        }

    }



    // for each command loop...


}

void Program::parseNew(QString input) {
    qInfo("NEW keyword");
}

void Program::parseTag(QString input) {
    qInfo("TAG keyword");
}

void Program::parseDelegate(QString input) {
    qInfo("DELEGATE keyword");
}

void Program::parseTitle(QString input) {
    qInfo("TITLE keyword");
}
