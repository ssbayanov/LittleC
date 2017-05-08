#ifndef STRUCTTABLERECORD_H
#define STRUCTTABLERECORD_H

#include "symbolstable/abstractsymboltablerecord.h"
#include "symbolstable/structtypetablerecord.h"

class StructTypeTableRecord;

class StructTableRecord : public AbstractSymbolTableRecord
{
public:
    StructTableRecord(QString name,
                      QString key,
                      SymbolsTable *table,
                      StructTypeTableRecord *structType);

    StructTypeTableRecord *getStructType();

private:
    StructTypeTableRecord *_structType;
};

#endif // STRUCTTABLERECORD_H
