#ifndef PROGRAM_H
#define PROGRAM_H

#include <QHash>
#include <QObject>
#include <QSocketNotifier>
#include <QTextStream>

enum INPUT { NEW, TAG, DELEGATE };

class Program : public QObject {

    Q_OBJECT

public:
    explicit Program(QObject *parent = nullptr);

private:
//    QSocketNotifier notifier;
    QTextStream inStream;

    QHash<QString, void (Program::*)(QString)> inputMap;

    bool parseRawLine(QString line);
    void parseNew(QString input);
    void createInputMap();
    void parseTag(QString input);
    void parseDelegate(QString input);
    void parseTitle(QString input);
    void parseDueDate(QString input);

    void parseTether(QString input);
    bool duplicateCheck(QStringList list);
    bool emptyCommandCheck(QHash<QString, QString> commands);
    void executeCommands(QHash<QString, QString> commands);
signals:

private slots:
    void notified();


};

#endif // PROGRAM_H
