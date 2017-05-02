#ifndef IRPRINT_H
#define IRPRINT_H

#include <QTextStream>

extern QTextStream outStream;

class IRPrint
{
public:
    IRPrint();

    QString writeCommandLine(QString textLine);

    QString lastCommandLine();

    void writeLine(QString textLine);
private:
    long int line;
};

#endif // IRPRINT_H
