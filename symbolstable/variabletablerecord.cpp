#include "variabletablerecord.h"

VariableTableRecord::VariableTableRecord(QString name, QString key, ValueTypeEnum valueType, QVariant value, SymbolsTable *table, SymbolsTableRecordType type)
    : AbstractSymbolTableRecord(type, name, key, valueType, table)
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
