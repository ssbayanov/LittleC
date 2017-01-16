#include "variabletablerecord.h"

VariableTableRecord::VariableTableRecord(QString name, ValueTypeEnum valueType, QVariant value, SymbolsTable *table)
    : AbstractSymbolTableRecord(SymbolTableRecord_Variable, name, valueType, value, table)
{

}

VariableTableRecord::~VariableTableRecord()
{

}
