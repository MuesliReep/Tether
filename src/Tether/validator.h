#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QString>

class Validator {

public:
    static bool validateLine(const QString& line);
private:
    static bool checkBrackets(const QString& line);
    static bool checkQuotes(const QString& line);

};

#endif // VALIDATOR_H
