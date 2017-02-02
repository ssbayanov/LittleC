#include "structtablerecord.h"

StructTableRecord::StructTableRecord(QString name, SymbolsTable *table, SymbolsTable *variables)
    : AbstractSymbolTableRecord(SymbolTableRecord_Function, name, typeStruct, table)
{
    _variables = variables;
}

SymbolsTable *StructTableRecord::getVariables()
{
    return _variables;
}



