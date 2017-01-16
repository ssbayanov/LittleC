#include "abstractsymboltablerecord.h"

AbstractSymbolTableRecord::AbstractSymbolTableRecord(SymbolsTableRecordType type, QString name, ValueTypeEnum valueType, SymbolsTable *table)
{
    _type = type;
    _name = name;
    _valueType = valueType;
    _table = table;
}

AbstractSymbolTableRecord::~AbstractSymbolTableRecord()
{

}

ValueTypeEnum AbstractSymbolTableRecord::getValueType()
{
    return _valueType;
}

QString AbstractSymbolTableRecord::getName()
{
    return _name;
}

SymbolsTable *AbstractSymbolTableRecord::getTable()
{
    return _table;
}

SymbolsTableRecordType AbstractSymbolTableRecord::getType()
{
    return _type;
}
