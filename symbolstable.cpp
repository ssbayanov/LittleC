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

SymbolsTableRecord * SymbolsTable::insertValue(QString name,
                                               ValueTypeEnum type,
                                               QVariant value,
                                               SymbolsTable *params)
{
    SymbolsTableRecord *record;
    try {
        record = new SymbolsTableRecord;
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    record->valueType = type;
    record->value = value;
    record->name = name;
    record->table = this;
    record->params = params;

    this->insert(name, record);

    return record;
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

SymbolsTableRecord *SymbolsTable::getVariable(QString name)
{
    SymbolsTable::iterator i = find(name);
    if(i != end())
        return (SymbolsTableRecord *) i.value();
    else
        return NULL;
}

SymbolsTableRecord *SymbolsTable::getVariableGlobal(QString name)
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




