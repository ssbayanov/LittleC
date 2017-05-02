#include "irprint.h"

IRPrint::IRPrint()
{
    line = 0;
    isStore = false;
    store.clear();
}

QString IRPrint::writeCommandLine(QString textLine)
{
    line++;
    QString tmp;
    tmp = QString("%%2 = %1\n")
            .arg(textLine.replace("%$t", "%"))
            .arg(line);
    if(isStore){
        store.last().append(tmp);
    }
    else {
        outStream << tmp;
    }

    return QString("%$t%1").arg(line);
}

QString IRPrint::writeLabelLine()
{
    line++;
    QString tmp;
    tmp = QString("; <label>: %%1\n")
            .arg(line);
    if(isStore){
        store.last().append(tmp);
    }
    else {
        outStream << tmp;
    }

    return QString("%$t%1").arg(line);
}

void IRPrint::writeLine(QString textLine)
{
    QString tmp;
    tmp =QString("%1\n")
            .arg(textLine.replace("%$t", "%"));
    if(isStore){
        store.last().append(tmp);
    }
    else {
        outStream << tmp;
    }
}

QString IRPrint::lastCommandLine()
{
    return QString("%$t%1").arg(line);
}


void IRPrint::startStore()
{
    isStore = true;
    store.append("");
}

void IRPrint::stopStore()
{
    if(!store.isEmpty()) {
        stoppedStore = store.last();
        store.removeAt(store.length()-1);
        if(store.isEmpty())
            isStore = false;
    }
}

void IRPrint::flushStore()
{
    if(store.isEmpty()) {
        outStream << stoppedStore.replace("%$t", "%");
        stoppedStore.clear();
    }
    else {
        store.last().append(stoppedStore.replace("%$t", "%"));
    }
    //store.clear();
}

void IRPrint::replaceInStored(QString what, QString on)
{
    if(stoppedStore.isEmpty())
        store.last().replace(what, on);
    else
        stoppedStore.replace(what, on);
}



