#include "symbolstable.h"
#include <QDebug>

SymbolsTable::SymbolsTable(SymbolsTable *p) :
    QList()
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

VariableTableRecord *SymbolsTable::insertVariable(QString name, QString key,
                                                  ValueTypeEnum type,
                                                  QVariant value,
                                                  SymbolsTableRecordType typeRecord)
{
    VariableTableRecord *record;
    try {
        record = new VariableTableRecord(name, key, type, value, this, typeRecord);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    this->append(record);

    return record;
}

VariableTableRecord *SymbolsTable::insertArray(QString name, QString key, ValueTypeEnum type)
{
    return insertVariable(name, key, type, 0, SymbolTableRecord_Array);
}

FunctionTableRecord *SymbolsTable::insertFunction(QString name, QString key, ValueTypeEnum type, SymbolsTable *params)
{
    FunctionTableRecord *function;
    try {
        function = new FunctionTableRecord(name, key, type, this, params);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    this->append(function);

    return function;
}

StructTypeTableRecord *SymbolsTable::insertStructType(QString name, QString key, SymbolsTable *variables)
{
    StructTypeTableRecord *structure;

    try {
        structure = new StructTypeTableRecord(name, key, this, variables);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    this->append(structure);

    return structure;

}

StructTableRecord *SymbolsTable::insertStruct(QString name, QString key, StructTypeTableRecord *typeStruct)
{
    StructTableRecord *structure;

    try {
        structure = new StructTableRecord(name, key, this, typeStruct);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }

    this->append(structure);

    return structure;
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
    for(SymbolsTable::iterator i = begin(); i != end(); i++) {
        if((*i)->getName() == name) {
            return *i;
        }
    }
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
        return ptr->getVariable(name);
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

bool SymbolsTable::contains(QString name) {

    if(getVariable(name) != NULL) {
        return true;
    }
    return false;
}

QList<SymbolsTable *> SymbolsTable::getChilds()
{
    return childs;
}


