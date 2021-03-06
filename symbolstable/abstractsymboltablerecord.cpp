#include "abstractsymboltablerecord.h"
#include "symbolstable/symbolstable.h"

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

QString AbstractSymbolTableRecord::getUniqueName()
{
    return _uniqueName;
}

void AbstractSymbolTableRecord::setUniqueName(QString name)
{
    _uniqueName = name;
}

SymbolsTable *AbstractSymbolTableRecord::getTable()
{
    return _table;
}

SymbolsTableRecordType AbstractSymbolTableRecord::getType()
{
    return _type;
}

QString AbstractSymbolTableRecord::getTypeName()
{
    if(isStruct()){
        return ((StructTableRecord*) this)->getStructName();
    }
    return typeName.at(_valueType);
}

int AbstractSymbolTableRecord::getSize()
{
    return getSizeType(_valueType);
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

bool AbstractSymbolTableRecord::isGlobal()
{
    return (getTable()->getParent() == NULL);
}
