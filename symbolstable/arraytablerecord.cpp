#include "arraytablerecord.h"

arraytablerecord::arraytablerecord(QString name, QString key, ValueTypeEnum valueType, SymbolsTable *table)
    : AbstractSymbolTableRecord(SymbolTableRecord_Array, name, key, valueType, table)
{

}

int arraytablerecord::getSize()
{
    return _size;
}
void arraytablerecord::setSize(int size)
{
    _size = size;
}
