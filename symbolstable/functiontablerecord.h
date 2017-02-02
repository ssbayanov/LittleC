#ifndef FUNCTIONTABLERECORD_H
#define FUNCTIONTABLERECORD_H

#include "symbolstable/abstractsymboltablerecord.h"

class SymbolsTable;

class FunctionTableRecord : public AbstractSymbolTableRecord
{
public:
    FunctionTableRecord(QString name, ValueTypeEnum valueType, SymbolsTable *table, SymbolsTable *params);
    ~FunctionTableRecord();

    SymbolsTable *getParams();

private:
    SymbolsTable *_params;
};

#endif // FUNCTIONTABLERECORD_H
