#ifndef FUNCTIONTABLERECORD_H
#define FUNCTIONTABLERECORD_H

#include "subexpression.h"
#include "symbolstable/abstractsymboltablerecord.h"
#include <QString>
#include <QVariant>

class SymbolsTable;

class FunctionTableRecord : public AbstractSymbolTableRecord
{
public:
    FunctionTableRecord(QString name, ValueTypeEnum valueType, SymbolsTable *table, SymbolsTable *params);
    ~FunctionTableRecord();

    SymbolsTable *getParams();
private:
    SymbolsTable *_params;
};

#endif // FUNCTIONTABLERECORD_H
