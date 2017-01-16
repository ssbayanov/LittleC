#include "symbolstable.h"
#include <QDebug>

SymbolsTable::SymbolsTable(SymbolsTable *p) :
    QHash()
{
    parent = p;
    if(parent != NULL)
        parent->childs.append(this);
    childs.clear();
}

SymbolsTable::~SymbolsTable()
{
    for(int i =0; i < childs.count(); i++)
        childs.at(i)->~SymbolsTable();

    clear();
    childs.clear();
}

VariableTableRecord *SymbolsTable::insertVariable(QString name,
                                                  ValueTypeEnum type,
                                                  QVariant value,
                                                  SymbolsTableRecordType typeRecord)
{
    VariableTableRecord *record;
    try {
        record = new VariableTableRecord(name, type, value, this, typeRecord);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    this->insert(name, record);

    return record;
}

VariableTableRecord *SymbolsTable::insertArray(QString name, ValueTypeEnum type)
{
    return insertVariable(name, type, 0, SymbolTableRecord_Array);
}

FunctionTableRecord *SymbolsTable::insertFunction(QString name, ValueTypeEnum type, SymbolsTable *params)
{
    FunctionTableRecord *function;
    try {
        function = new FunctionTableRecord(name, type, this, params);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    this->insert(name, function);

    return function;
}

void SymbolsTable::setParent(SymbolsTable *p)
{
    parent = p;
}

SymbolsTable *SymbolsTable::getParent()
{
    return parent;
}

void SymbolsTable::setHidden(bool flag)
{
    _isHidden = flag;
}

bool SymbolsTable::isHidden()
{
    return _isHidden;
}

void SymbolsTable::insertChild(SymbolsTable *c)
{
    childs.append(c);
}

u_int SymbolsTable::getChildsCount()
{
    return childs.count();
}

SymbolsTable *SymbolsTable::appendChildTable()
{
    SymbolsTable *newTable;
    try {
        newTable = new SymbolsTable(this);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    return newTable;
}

AbstractSymbolTableRecord *SymbolsTable::getVariable(QString name)
{
    SymbolsTable::iterator i = find(name);
    if(i != end())
        return (AbstractSymbolTableRecord *) i.value();
    else
        return NULL;
}

AbstractSymbolTableRecord *SymbolsTable::getVariableGlobal(QString name)
{
    SymbolsTable *ptr = this;
    while(ptr != NULL && !ptr->contains(name))
    {
        ptr = ptr->getParent();
    }
    if(ptr != NULL){
        return ptr->find(name).value();
    }
    return NULL;
}

bool SymbolsTable::containsGlobal(QString name)
{
    SymbolsTable *ptr = this;
    while(ptr != NULL && !ptr->contains(name))
    {
        ptr = ptr->getParent();
    }
    if(ptr != NULL)
        return true;
    return false;
}

