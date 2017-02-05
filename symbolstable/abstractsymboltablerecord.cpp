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

bool AbstractSymbolTableRecord::isFunction()
{
    return (_type == SymbolTableRecord_Function);
}

bool AbstractSymbolTableRecord::isArray()
{
    return (_type == SymbolTableRecord_Array);
}

bool AbstractSymbolTableRecord::isVariable()
{
    return (_type == SymbolTableRecord_Variable);
}

bool AbstractSymbolTableRecord::isStructType()
{
    return (_type == SymbolTableRecord_StructType);
}

bool AbstractSymbolTableRecord::isStruct()
{
    return (_type == SymbolTableRecord_Struct);
}
