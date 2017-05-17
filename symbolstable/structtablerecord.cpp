#include "structtablerecord.h"
#include "symbolstable.h"

StructTableRecord::StructTableRecord(QString name, SymbolsTable *table, StructTypeTableRecord *structType)
    : AbstractSymbolTableRecord(SymbolTableRecord_Struct, name, typeStruct, table)
{
    _structType = structType;
}

StructTypeTableRecord *StructTableRecord::getStructType()
{
    return _structType;
}

QString StructTableRecord::getStructName()
{
    return _structType->getName();
}

uint StructTableRecord::getVarIndex(AbstractSymbolTableRecord *var)
{
    return _structType->getVariables()->indexOf(var);
}

