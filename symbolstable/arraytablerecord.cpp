#include "arraytablerecord.h"

ArrayTableRecord::ArrayTableRecord(QString name, QString key, ValueTypeEnum valueType, SymbolsTable *table)
    : AbstractSymbolTableRecord(SymbolTableRecord_Array, name, key, valueType, table)
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
