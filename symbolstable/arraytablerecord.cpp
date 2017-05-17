#include "arraytablerecord.h"

ArrayTableRecord::ArrayTableRecord(QString name, ValueTypeEnum valueType, SymbolsTable *table)
    : AbstractSymbolTableRecord(SymbolTableRecord_Array, name, valueType, table)
{

}

int ArrayTableRecord::getSize()
{
    return _size;
}
void ArrayTableRecord::setSize(int size)
{
    _size = size;
}
