#include "irprint.h"

IRPrint::IRPrint()
{
    line = 0;
    isStore = false;
    store.clear();
    currentSpace = new Space {LabelStore(),QString()};
    store.append(currentSpace);
}

QString IRPrint::writeCommandLine(QString textLine)
{
    line++;
    writeLine(QString("%%2 = %1\n")
            .arg(textLine)
            .arg(line)
            .replace("$t", "%"));

    return QString("$t%1").arg(line);
}

QString IRPrint::writeLabelLine(QString comment)
{
    line++;
    QString tmp;
    tmp = QString("\n; <label>: %%2%1\n")
            .arg(comment.isEmpty() ? "" : QString("\t\t\t#%1")
                                                 .arg(comment))
            .arg(line);
    currentSpace->text.append(tmp);


    return QString("$t%1").arg(line);
}

QString IRPrint::writeNamedLabelLine(QString name)
{
    currentSpace->labelsStore.insert(name, writeLabelLine(name));
    return lastCommandLine();
}

QString IRPrint::getLabelByName(QString name)
{
    return currentSpace->labelsStore.value(name);
}

void IRPrint::writeLine(QString textLine)
{
    QString lastLine = currentSpace->text.right(currentSpace->text.length() - currentSpace->text.lastIndexOf("\n", -2));
    if(!textLine.split(" ").contains("br") ||
            !lastLine.contains("\nbr "))
    currentSpace->text.append(QString("%1\n")
                              .arg(textLine.replace("$t", "%")));
}

QString IRPrint::lastCommandLine()
{
    return QString("$t%1").arg(line);
}


void IRPrint::startStore()
{
    isStore = true;

    currentSpace = new Space {LabelStore(),QString()};
    store.append(currentSpace);
}

void IRPrint::stopStore()
{
    if(!store.isEmpty()) {
        //store.removeAt(store.length()-1);
    }
}

void IRPrint::flushStore()
{
    store.removeAt(store.length()-1);

    if(store.isEmpty()) {
        outStream << currentSpace->text
                     .replace("$t", "%");
    }
    else {
        store.last()->text.append(currentSpace->text.replace("$t", "%"));
        delete currentSpace;
        currentSpace = store.last();
    }
    //store.clear();
}

void IRPrint::replaceInStored(QString what, QString on)
{
    currentSpace->text = currentSpace->text.replace(what, on);
}
