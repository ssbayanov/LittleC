#ifndef ABSTRACTSYMBOLTABLERECORD_H
#define ABSTRACTSYMBOLTABLERECORD_H
#include "subexpression.h"
#include <QString>
#include <QVariant>

class SymbolsTable;

typedef enum {
    SymbolTableRecord_Function,
    SymbolTableRecord_Variable,
    SymbolTableRecord_Array,
    SymbolTableRecord_Struct,
    SymbolTableRecord_StructType
} SymbolsTableRecordType;

class AbstractSymbolTableRecord
{
public:
    AbstractSymbolTableRecord(SymbolsTableRecordType type,
                              QString name,
                              ValueTypeEnum valueType,
                              SymbolsTable *table);
    ~AbstractSymbolTableRecord();

    ValueTypeEnum getValueType();
    QString getName();
    QString getUniqueName();
    void setUniqueName(QString name);

    SymbolsTable *getTable();
    SymbolsTableRecordType getType();
    QString getTypeName();
    int getSize();

    bool isFunction();
    bool isArray();
    bool isVariable();
    bool isStructType();
    bool isStruct();

    bool isGlobal();

protected:
    ValueTypeEnum _valueType; /* Type of a variable or expression */
    QString _name;
    QString _uniqueName;
    SymbolsTable *_table;

    SymbolsTableRecordType _type;
};

#endif // ABSTRACTSYMBOLTABLERECORD_H
