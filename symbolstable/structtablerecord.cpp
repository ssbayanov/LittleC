#include "structtablerecord.h"

StructTableRecord::StructTableRecord(QString name, QString key, SymbolsTable *table, StructTypeTableRecord *structType)
    : AbstractSymbolTableRecord(SymbolTableRecord_Struct, name, key, typeStruct, table)
{
    _structType = structType;
}

StructTypeTableRecord *StructTableRecord::getStructType()
{
    return _structType;
}

