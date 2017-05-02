#ifndef IRPRINT_H
#define IRPRINT_H

#include <QTextStream>

extern QTextStream outStream;

class IRPrint
{
public:
    IRPrint();

    QString writeCommandLine(QString textLine);

    QString writeLabelLine();

    QString lastCommandLine();

    void writeLine(QString textLine);

    void startStore();

    void stopStore();

    void flushStore();

    void replaceInStored(QString what, QString on);

private:
    long int line;
    bool isStore;

    QStringList store;
    QString stoppedStore;
};

#endif // IRPRINT_H
