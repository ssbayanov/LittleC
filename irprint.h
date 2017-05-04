#ifndef IRPRINT_H
#define IRPRINT_H

#include <QTextStream>
#include <QHash>

extern QTextStream outStream;

typedef QHash<QString, QString> LabelStore;

struct Space{
    LabelStore labelsStore;
    QString text;
};

class IRPrint
{
public:
    IRPrint();

    QString writeCommandLine(QString textLine);

    QString writeLabelLine(QString comment = "");

    QString writeNamedLabelLine(QString name);

    QString getLabelByName(QString name);

    QString lastCommandLine();

    void writeLine(QString textLine);

    void startStore();

    void stopStore();

    void flushStore();

    void replaceInStored(QString what, QString on);

private:
    long int line;
    bool isStore;

    //QStringList store;
    //QString stoppedStore;

    QList<Space *> store;

    Space *currentSpace;

};

#endif // IRPRINT_H
