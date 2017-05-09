#ifndef IRPRINT_H
#define IRPRINT_H

#include <QTextStream>
#include <QHash>

extern QTextStream outStream;

typedef QHash<QString, QString> LabelStore;

class VariablesStore : public QHash<QString, QString>
{
public:
    VariablesStore();
    QString getVariablesText();
};

struct Space{
    LabelStore labelsStore;
    LabelStore userLabelsStore;
    QString text;
};

class IRPrint
{
public:
    IRPrint();

    QString writeCommandLine(QString textLine);

    QString writeLabelLine(QString comment = "");

    QString writeNamedLabelLine(QString name);

    QString writeUserLabelLine(QString name);

    QString getLabelByName(QString name);

    QString getUserLabelByName(QString name);

    QString lastCommandLine();

    QString addUnnamedVariable(QString declarationText);

    QString getUniqueNameAndStore(QString name);

    void writeLine(QString textLine);

    void writeGlobalLine(QString textLine);

    void addExternFunction(QString textLine);

    void startStore();

    void stopStore();

    void flushStore();

    void replaceInStored(QString what, QString on);

    void resetCommandCounter();

private:
    long int line;

    long int unnameVarsCount;

    bool isStore;

    QString globalText;

    QStringList externFunctions;

    VariablesStore unnamedVariables;

    QList<Space *> store;

    QStringList varStore;

    Space *currentSpace;

};

#endif // IRPRINT_H
