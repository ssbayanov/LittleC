#include "structtablerecord.h"

StructTableRecord::StructTableRecord(QString name, SymbolsTable *table, StructTypeTableRecord *structType)
    : AbstractSymbolTableRecord(SymbolTableRecord_Struct, name, typeStruct, table)
{
    _structType = structType;
}

StructTypeTableRecord *StructTableRecord::getStructType()
{
    return _structType;
}

