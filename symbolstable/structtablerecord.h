#ifndef STRUCTTABLERECORD_H
#define STRUCTTABLERECORD_H

#include "symbolstable/abstractsymboltablerecord.h"

class StructTableRecord : public AbstractSymbolTableRecord
{
public:
    StructTableRecord(QString name, SymbolsTable *table, SymbolsTable *variables);

    SymbolsTable *getVariables();

private:
    SymbolsTable *_variables;
};

#endif // STRUCTTABLERECORD_H
