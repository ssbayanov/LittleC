#ifndef FUNCTIONTABLERECORD_H
#define FUNCTIONTABLERECORD_H

#include "subexpression.h"
//#include "symbolstable/symbolstable.h"
#include "symbolstable/abstractsymboltablerecord.h"
#include <QString>
#include <QVariant>

class SymbolsTable;
//class AbstractSymbolTableRecord;

class FunctionTableRecord : public AbstractSymbolTableRecord
{
public:
    FunctionTableRecord(QString name, ValueTypeEnum valueType, QVariant value, SymbolsTable *table, SymbolsTable *params);
    ~FunctionTableRecord();

    SymbolsTable *getParams();
private:
    SymbolsTable *_params;
};

#endif // FUNCTIONTABLERECORD_H
