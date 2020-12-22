#include "program.h"

#include <QRegularExpression>
#include <QTextStream>

Program::Program(QObject *parent) : QObject(parent)
//  ,     notifier(STDIN_FILENO, QSocketNotifier::Read), inStream(stdin)
{

//    connect(&notifier, &QSocketNotifier::activated, this, &Program::notified);

//    connect(inStream.device(), &QIODevice::readyRead, this, &Program::notified);

    createInputMap();


    parse("test");
}

void Program::createInputMap() {

    inputMap["NEW"]         = &Program::parseNew;
    inputMap["TAG"]         = &Program::parseTag;
    inputMap["DELEGATE"]    = &Program::parseDelegate;
    inputMap["TITLE"]       = &Program::parseTitle;
    inputMap["DUE"]         = &Program::parseDueDate;
    inputMap["TETHER"]      = &Program::parseTether;
}

void Program::notified() {

    qDebug("received");
}

void Program::parse(QString line) {

    line = "[NEW] aaa aaa [TITLE]bbb aaa [DELEGATE] ccc";


//    QList<QString> duplicateCheck;



//    QRegularExpression commandInputEx("(\\[\\w+\\]( |)\\w+)");

    // Debug with: https://regex101.com/
    QRegularExpression commandInputEx("(\\[\\w+\\]( |)(\\w+( |)){1,})");
    QRegularExpression commandEx("(\\[\\w+\\])");

    QRegularExpressionMatchIterator matchIterator = commandInputEx.globalMatch(line);

    while(matchIterator.hasNext()) {

        QRegularExpressionMatch match = matchIterator.next();
        QString word = match.captured(1);

//        qInfo(word.toUtf8());

        QRegularExpressionMatch commandMatch = commandEx.match(word);

        if(commandMatch.hasMatch()) {

            //
            QString command = commandMatch.captured(0);

            //
            QString input = word.remove(command);

            // Remove brackets
            command.remove('[');
            command.remove(']');

            // Parse it
            inputMap.contains(command) ? (this->*inputMap.value(command))(input)  : qInfo("Invalid Command!");
        }else {
            qInfo("No command match!");
        }

    }



    // for each command loop...


}

void Program::parseTether(QString input) {
    qInfo("TETHER keyword, input: %s", input.toStdString().c_str());
}

void Program::parseDueDate(QString input) {
    qInfo("DUE keyword, input: %s", input.toStdString().c_str());
}

void Program::parseNew(QString input) {
    qInfo("NEW keyword, input: %s", input.toStdString().c_str());
}

void Program::parseTag(QString input) {
    qInfo("TAG keyword, input: %s", input.toStdString().c_str());
}

void Program::parseDelegate(QString input) {
    qInfo("DELEGATE keyword, input: %s", input.toStdString().c_str());
}

void Program::parseTitle(QString input) {
    qInfo("TITLE keyword, input: %s", input.toStdString().c_str());
}
