#include "structtypetablerecord.h"

StructTypeTableRecord::StructTypeTableRecord(QString name, SymbolsTable *table, SymbolsTable *variables)
    : AbstractSymbolTableRecord(SymbolTableRecord_StructType, name, typeStructType, table)
{
    _variables = variables;
}

SymbolsTable *StructTypeTableRecord::getVariables()
{
    return _variables;
}



