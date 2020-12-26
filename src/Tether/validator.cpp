#include "validator.h"


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

    bool result;

    int numQuotes = line.count("\"");

    (numQuotes % 2 == 0) ? result = true :  result = false;

    return result;
}
