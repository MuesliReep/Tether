#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QString>
#include <QHash>

class Validator {

public:
    static bool validateLine(const QString& line);
    static QString getValidCommandDescription(QString);
    static QStringList getValidCommands();
    static QStringList getMandatoryUniqueCommands();

private:
    static bool checkBrackets(const QString& line);
    static bool checkQuotes(const QString& line);


};

#endif // VALIDATOR_H
