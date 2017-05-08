#ifndef ARRAYTABLERECORD_H
#define ARRAYTABLERECORD_H
#include "symbolstable/abstractsymboltablerecord.h"


class arraytablerecord : public AbstractSymbolTableRecord
{
public:
    arraytablerecord(QString name,
                     QString key, ValueTypeEnum valueType,
                     SymbolsTable *table);

 int getSize();
 void setSize(int size);

private:
 int _size;
};

#endif // ARRAYTABLERECORD_H
