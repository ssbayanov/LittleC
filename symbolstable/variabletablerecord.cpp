#include "variabletablerecord.h"

VariableTableRecord::VariableTableRecord(QString name, ValueTypeEnum valueType, QVariant value, SymbolsTable *table, SymbolsTableRecordType type)
    : AbstractSymbolTableRecord(type, name, valueType, table)
{
    _value = value;
}

VariableTableRecord::~VariableTableRecord()
{

}

QVariant VariableTableRecord::getValue()
{
    return _value;
}
