#ifndef ABSTRACTSYMBOLTABLERECORD_H
#define ABSTRACTSYMBOLTABLERECORD_H
#include "subexpression.h"
//#include "symbolstable.h"
#include <QString>
#include <QVariant>

class SymbolsTable;

typedef enum {
    SymbolTableRecord_Function,
    SymbolTableRecord_Variable
} SymbolsTableRecordType;

class AbstractSymbolTableRecord
{
public:
    AbstractSymbolTableRecord(SymbolsTableRecordType type, QString name, ValueTypeEnum valueType, QVariant value, SymbolsTable *table);
    ~AbstractSymbolTableRecord();

    ValueTypeEnum getValueType();
    QString getName();
    SymbolsTable *getTable();

    SymbolsTableRecordType getType();

private:
    ValueTypeEnum _valueType; /* Type of a variable or expression */
    QVariant _value;   /* Currently not used, reserved to the future */
    QString _name;
    SymbolsTable *_table;

    SymbolsTableRecordType _type;
};

#endif // ABSTRACTSYMBOLTABLERECORD_H
