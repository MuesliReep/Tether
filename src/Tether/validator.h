#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QString>

class Validator
{

public:
    static bool validateLine(QString line);
private:
    static bool checkBrackets(QString line);
    static bool checkQuotes(QString line);
};

#endif // VALIDATOR_H
