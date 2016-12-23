#ifndef SYMBOLSTABLE_H
#define SYMBOLSTABLE_H

#include <QHash>
#include <QString>
#include <QVariant>
#include <QList>
#include "subexpression.h"

class SymbolsTable;

// Symbols table record definition
typedef struct
{
  SubexpressionValueTypeEnum valueType; /* Type of a variable or expression */
  QVariant value;   /* Currently not used, reserved to the future */
  QString name;
  SymbolsTable *table;
} SymbolsTableRecord;

class SymbolsTable : public QHash<QString, SymbolsTableRecord*>
{
public:
    SymbolsTable(SymbolsTable *p);

    // Hidden
    void setHidden(bool flag = true);
    bool isHidden();

    // Parent
    void setParent(SymbolsTable *p);
    SymbolsTable *getParent();

    // Childs
    bool insertChild(SymbolsTable *c);
    u_int getChildsCount();
    SymbolsTable *appendChildTable();

    // Variables
    SymbolsTableRecord *insertValue(QString name,
                      SubexpressionValueTypeEnum type,
                      QVariant value);
    SymbolsTableRecord* getVariable(QString name);
    SymbolsTableRecord* getVariableGlobal(QString name);
    bool containsGlobal(QString name);


private:
    SymbolsTable *parent;
    QList<SymbolsTable*> childs;
    bool _isHidden;
};

#endif // SYMBOLSTABLE_H
