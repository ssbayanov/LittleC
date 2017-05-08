#include "functiontablerecord.h"
#include "symbolstable/symbolstable.h"

FunctionTableRecord::FunctionTableRecord(QString name, QString key, ValueTypeEnum valueType, SymbolsTable *table, SymbolsTable *params)
    : AbstractSymbolTableRecord(SymbolTableRecord_Function, name, key, valueType, table)
{
    _params = params;
}

SymbolsTable *FunctionTableRecord::getParams()
{
    return _params;
}

