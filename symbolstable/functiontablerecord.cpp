#include "functiontablerecord.h"

FunctionTableRecord::FunctionTableRecord(QString name, ValueTypeEnum valueType, SymbolsTable *table, SymbolsTable *params)
    : AbstractSymbolTableRecord(SymbolTableRecord_Function, name, valueType, table)
{
    _params = params;
}

SymbolsTable *FunctionTableRecord::getParams()
{
    return _params;
}
