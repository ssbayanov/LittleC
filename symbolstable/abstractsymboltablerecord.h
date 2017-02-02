#ifndef ABSTRACTSYMBOLTABLERECORD_H
#define ABSTRACTSYMBOLTABLERECORD_H
#include "subexpression.h"
#include <QString>
#include <QVariant>

class SymbolsTable;

typedef enum {
    SymbolTableRecord_Function,
    SymbolTableRecord_Variable,
    SymbolTableRecord_Array
} SymbolsTableRecordType;

class AbstractSymbolTableRecord
{
public:
    AbstractSymbolTableRecord(SymbolsTableRecordType type, QString name, ValueTypeEnum valueType, SymbolsTable *table);
    ~AbstractSymbolTableRecord();

    ValueTypeEnum getValueType();
    QString getName();

    SymbolsTable *getTable();
    SymbolsTableRecordType getType();

    bool isFunction();
    bool isArray();
    bool isVariable();

protected:
    ValueTypeEnum _valueType; /* Type of a variable or expression */
    QString _name;
    SymbolsTable *_table;

    SymbolsTableRecordType _type;
};

#endif // ABSTRACTSYMBOLTABLERECORD_H
