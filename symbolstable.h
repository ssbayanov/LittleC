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
  ValueTypeEnum valueType; /* Type of a variable or expression */
  QVariant value;   /* Currently not used, reserved to the future */
  QString name;
  SymbolsTable *table;
} SymbolsTableRecord;

/**
 * @brief The SymbolsTable class for store variables in diffrence scopes
 */
class SymbolsTable : public QHash<QString, SymbolsTableRecord*>
{
public:
    SymbolsTable(SymbolsTable *p);

    ~SymbolsTable();

    // Hidden
    /**
     * @brief setHidden function for change hidden flag
     * @param flag hidden parametr
     */
    void setHidden(bool flag = true);

    /**
     * @brief isHidden function for inspection hidden flag
     * @return status of hidden flag
     */
    bool isHidden();

    // Parent
    /**
     * @brief setParent set parent table for this symbols table
     * @param p - pointer to parent table
     */
    void setParent(SymbolsTable *p);

    /**
     * @brief getParent
     * @return return pointer to current parent table
     */
    SymbolsTable *getParent();

    // Childs
    /**
     * @brief insertChild inserting child table in childs list
     * @param c pointer to current table
     * @return true if insert accepted else return false
     */
    bool insertChild(SymbolsTable *c);

    /**
     * @brief getChildsCount
     * @return count of child list
     */
    u_int getChildsCount();

    /**
     * @brief appendChildTable append empty child table
     * @return pointer to new SymbolsTable
     */
    SymbolsTable *appendChildTable();

    // Variables
    /**
     * @brief insertValue inserting new value into values list
     * @param name name of variable
     * @param type type of variable
     * @param value value of variable
     * @return pointer to struct of this variable
     */
    SymbolsTableRecord *insertValue(QString name,
                      ValueTypeEnum type,
                      QVariant value);
    /**
     * @brief getVariable find variable into values list by name in current table
     * @param name searching variable name
     * @return pointer to struct of searching variable. If searching failed return NULL
     */
    SymbolsTableRecord* getVariable(QString name);
    /**
     * @brief getVariableGlobal find variable into values list by name in current table and all parents
     * @param name searching variable name
     * @return pointer to struct of searching variable. If searching failed return NULL
     */
    SymbolsTableRecord* getVariableGlobal(QString name);
    /**
     * @brief containsGlobal find variable into values list by name in current table and all parents
     * @param name searching variable name
     * @return true if variable found or false if variable not founded
     */
    bool containsGlobal(QString name);


private:
    SymbolsTable *parent;
    QList<SymbolsTable*> childs;
    bool _isHidden;
};

#endif // SYMBOLSTABLE_H
