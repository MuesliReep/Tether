#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QString>

class Validator
{
public:
    Validator();
public:
    static bool validateLine(QString line);
};

#endif // VALIDATOR_H
