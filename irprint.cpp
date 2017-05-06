#include "irprint.h"

IRPrint::IRPrint()
{
    line = 0;
    unnameVarsCount = 0;
    isStore = false;
    store.clear();
    externFunctions.clear();
    currentSpace = new Space {LabelStore(),QString()};
    store.append(currentSpace);

    writeLine("%global_placed_here%");
}

QString IRPrint::writeCommandLine(QString textLine)
{
    line++;
    writeLine(QString("%%2 = %1")
              .arg(textLine)
              .arg(line));

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
    //bug with double br
    QString lastLine = currentSpace->text.right(currentSpace->text.length() - currentSpace->text.lastIndexOf("\n", -4));
    if(!textLine.split(" ").contains("br") ||
            (!lastLine.contains("\nbr ") && !lastLine.contains("\nret ")))
        currentSpace->text.append(QString("%1\n")
                                  .arg(textLine.replace("$t", "%")));
}

void IRPrint::writeGlobalLine(QString textLine)
{
    globalText.append(textLine + "\n");
}

void IRPrint::addExternFunction(QString textLine)
{
    if(!externFunctions.contains(textLine))
        externFunctions.append(textLine);
}

QString IRPrint::lastCommandLine()
{
    return QString("$t%1").arg(line);
}

QString IRPrint::addUnnamedVariable(QString declarationText)
{
    if(unnamedVariables.contains(declarationText))
        return unnamedVariables.key(declarationText);

    unnamedVariables.insert(QString("@unname_variable_%1")
                            .arg(unnameVarsCount++)
                            , declarationText);

    return unnamedVariables.key(declarationText);
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
        globalText.append(unnamedVariables.getVariablesText());

        QString externText = "";
        if(!externFunctions.isEmpty()) {
            if(externFunctions.count() != 1){
                externText = externFunctions.join("\n");
            }
            else{
                externText = externFunctions.at(0);
            }
        }

        outStream << currentSpace->text
                     .replace("$t", "%")
                     .replace("%global_placed_here%", globalText)
                  << externText;
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

void IRPrint::resetCommandCounter()
{
    line = 0;
}

VariablesStore::VariablesStore()
{
    this->clear();
}

QString VariablesStore::getVariablesText()
{
    QString tmpText = "";

    iterator i = begin();
    while (i != end()) {
        tmpText.append(i.key()+" = "+i.value()+"\n");
        ++i;
    }

    return tmpText;
}
