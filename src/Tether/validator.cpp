#include "validator.h"

#include <QStringList>


bool Validator::validateLine(const QString& line) {

    // First do some easy checks
    if(!checkBrackets(line)) {
        return false;
    }

    //
    if(!checkBrackets(line)) {
        return false;
    }

    return true;
}

QStringList Validator::getValidCommands() {

    QStringList validCommands;

    validCommands.append("NEW");
    validCommands.append("TAG");
    validCommands.append("DELEGATE");
    validCommands.append("TITLE");
    validCommands.append("DUE");
    validCommands.append("TETHER");
    validCommands.append("MODIFY");
    validCommands.append("DELETE");

    return validCommands;
}

QStringList Validator::getMandatoryUniqueCommands() {

    QStringList mandatoryUniqueCommands;

    mandatoryUniqueCommands.append("NEW");
    mandatoryUniqueCommands.append("MODIFY");
    mandatoryUniqueCommands.append("DELETE");

    return mandatoryUniqueCommands;
}

QHash<QString, QString> Validator::getValidCommandDescriptions() {

    QHash<QString, QString> commandDescriptions;

    commandDescriptions["NEW"] = "Create a new item";
    commandDescriptions["TAG"] = "Set a searchable tag";
    commandDescriptions["DELEGATE"] = "Delegate this item to another person";
    commandDescriptions["TITLE"] = "Set a title for this item";
    commandDescriptions["DUE"] = "Set a due date";
    commandDescriptions["TETHER"] = "Directly link this item to another";
    commandDescriptions["MODIFY"] = "Modify parts of an existing item";
    commandDescriptions["DELETE"] = "Deletes an existing item";

    return commandDescriptions;
}

QString Validator::getValidCommandDescription(QString command) {

    QHash<QString, QString> commandDescriptions = getValidCommandDescriptions();

    QString result = "Unknown";

    if(commandDescriptions.contains(command)) {
        result = commandDescriptions.value(command);
    }

    return result;
}

// Checks if the same number of opening and closing brackets has been used
bool Validator::checkBrackets(const QString& line) {

    int openingBrackets = line.count("[");
    int closingBrackets = line.count("]");

    if(openingBrackets != closingBrackets) {
        return false;
    }

    return true;
}

// Checks if an even number of quotes has been used
bool Validator::checkQuotes(const QString& line) {

    bool result = false;

    int numQuotes = line.count("\"");

    (numQuotes % 2 == 0) ? result = true :  result = false;

    return result;
}
