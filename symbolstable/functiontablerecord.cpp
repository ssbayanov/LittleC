#include "functiontablerecord.h"

FunctionTableRecord::FunctionTableRecord(QString name, ValueTypeEnum valueType, QVariant value, SymbolsTable *table, SymbolsTable *params)
    : AbstractSymbolTableRecord(SymbolTableRecord_Function, name, valueType, value, table)
{
    _params = params;
}

SymbolsTable *FunctionTableRecord::getParams()
{
    return _params;
}
