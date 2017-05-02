#include "irprint.h"

IRPrint::IRPrint()
{
    line = 0;
}

QString IRPrint::writeCommandLine(QString textLine)
{
    line++;
    outStream << "%" << line << " = " << textLine.replace("%~t", "%") << "\n";
    return QString("%~t%1").arg(line);
}

QString IRPrint::lastCommandLine()
{
    return QString("%~t%1").arg(line);
}

void IRPrint::writeLine(QString textLine)
{
    outStream << textLine.replace("%~t", "%") << "\n";
}

