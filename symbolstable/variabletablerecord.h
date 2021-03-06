#ifndef VARIABLETABLERECORD_H
#define VARIABLETABLERECORD_H

#include "symbolstable/abstractsymboltablerecord.h"

#include <QString>
#include <QVariant>

class SymbolsTable;

class VariableTableRecord : public AbstractSymbolTableRecord
{
public:
    VariableTableRecord(QString name,
                        ValueTypeEnum valueType,
                        QVariant value, SymbolsTable *table,
                        SymbolsTableRecordType type = SymbolTableRecord_Variable);
    ~VariableTableRecord();

    QVariant getValue();

private:
    QVariant _value;   /* Currently not used, reserved to the future */
};

#endif // VARIABLETABLERECORD_H
