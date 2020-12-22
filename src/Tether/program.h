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
    QSocketNotifier notifier;
    QTextStream inStream;

    QHash<QString, void (Program::*)(QString)> inputMap;

    void parse(QString line);
    void parseNew(QString input);
    void createInputMap();
    void parseTag(QString input);
    void parseDelegate(QString input);
    void parseTitle(QString input);
signals:

private slots:
    void notified();


};

#endif // PROGRAM_H
