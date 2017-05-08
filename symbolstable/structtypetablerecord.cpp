#include "structtypetablerecord.h"

StructTypeTableRecord::StructTypeTableRecord(QString name, QString key, SymbolsTable *table, SymbolsTable *variables)
    : AbstractSymbolTableRecord(SymbolTableRecord_StructType, name, key, typeStructType, table)
{
    _variables = variables;
}

SymbolsTable *StructTypeTableRecord::getVariables()
{
    return _variables;
}



