#include "program.h"

#include <QRegularExpression>
#include <QTextStream>

Program::Program(QObject *parent) : QObject(parent)
//  ,     notifier(STDIN_FILENO, QSocketNotifier::Read), inStream(stdin)
{

//    connect(&notifier, &QSocketNotifier::activated, this, &Program::notified);

//    connect(inStream.device(), &QIODevice::readyRead, this, &Program::notified);

    createInputMap();


    parseRawLine("test");
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

// Returns true if duplicates have been found
bool Program::duplicateCheck(QStringList list) {

    foreach (QString command, list) {
        if(list.count(command) > 1) {
            qInfo("Duplicate command found: %s", command.toStdString().c_str());
            return true;
        }
    }

    return false;
}

bool Program::parseRawLine(QString line) {

    line = "[NEW] aaa aaa [TITLE]bbb aaa [DELEGATE] ccc [TETHER]     a [TITLE] cccc";

    QStringList duplicateList;
    QHash<QString, QString> commands;

//    QRegularExpression commandInputEx("(\\[\\w+\\]( |)\\w+)");

    // Debug with: https://regex101.com/
    // Ignores empty commands
    QRegularExpression commandWithInputEx("(\\[\\w+\\]( ){0,}(\\w+( |)){1,})");
    QRegularExpression commandEx("(\\[\\w+\\])");

    QRegularExpressionMatchIterator matchIterator = commandWithInputEx.globalMatch(line);

    while(matchIterator.hasNext()) {

        QRegularExpressionMatch match = matchIterator.next();
        QString word = match.captured(1);

        QRegularExpressionMatch commandMatch = commandEx.match(word);

        if(commandMatch.hasMatch()) {

            //
            QString command = commandMatch.captured(0);

            //
            QString input = word.remove(command);

            // Remove brackets
            command.remove('[');
            command.remove(']');

            // Put each command in a list to check for duplicates later
            duplicateList.append(command);

            //
            commands[command] = input;

        }else {
            qInfo("No command match!");
        }

    }

    // Check for duplicates
    duplicateCheck(duplicateList);

    // Check for empty commands
    emptyCommandCheck(commands);

    // Run the commands
    executeCommands(commands);


    return true;
}

void Program::executeCommands(QHash<QString, QString> commands) {

    // TODO: Iterate over QHash
//    inputMap.contains(command) ? (this->*inputMap.value(command))(input)  : qInfo("Invalid Command!");
}

bool Program::emptyCommandCheck(QHash<QString, QString> commands) {

    // If we remove all the spaces does the input still exist?
    foreach (QString input, commands) {
        input.remove(" ");
        if(input.isEmpty()) {
            qInfo("Empty command detected");
            return true;
        }
    }

    return false;
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
