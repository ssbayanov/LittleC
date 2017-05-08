#ifndef STRUCTTYPETABLERECORD_H
#define STRUCTTYPETABLERECORD_H

#include "symbolstable/abstractsymboltablerecord.h"

class StructTypeTableRecord : public AbstractSymbolTableRecord
{
public:
    StructTypeTableRecord(QString name,
                          QString key,
                          SymbolsTable *table,
                          SymbolsTable *variables);

    SymbolsTable *getVariables();

private:
    SymbolsTable *_variables;
};

#endif // STRUCTTYPETABLERECORD_H
