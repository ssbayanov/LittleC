#ifndef VARIABLETABLERECORD_H
#define VARIABLETABLERECORD_H

#include "subexpression.h"
#include "symbolstable/abstractsymboltablerecord.h"
//#include "symbolstable/symbolstable.h"

#include <QString>
#include <QVariant>

class SymbolsTable;

class VariableTableRecord : public AbstractSymbolTableRecord
{
public:
    VariableTableRecord(QString name, ValueTypeEnum valueType, QVariant value, SymbolsTable *table);
    ~VariableTableRecord();
};

#endif // VARIABLETABLERECORD_H
