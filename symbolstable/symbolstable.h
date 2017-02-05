#ifndef SYMBOLSTABLE_H
#define SYMBOLSTABLE_H

#include <QString>
#include <QVariant>
#include <QList>
#include "subexpression.h"
#include "symbolstable/variabletablerecord.h"
#include "symbolstable/functiontablerecord.h"
#include "symbolstable/abstractsymboltablerecord.h"
#include "symbolstable/structtablerecord.h"
#include "symbolstable/structtypetablerecord.h"



/**
 * @brief The SymbolsTable class for store variables in diffrence scopes
 */
class SymbolsTable : public QList<AbstractSymbolTableRecord*>
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
    void insertChild(SymbolsTable *c);

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
    VariableTableRecord *insertVariable(QString name,
                                        ValueTypeEnum type,
                                        QVariant value,
                                        SymbolsTableRecordType typeRecord = SymbolTableRecord_Variable);

    VariableTableRecord *insertArray(QString name,
                                     ValueTypeEnum type);

    FunctionTableRecord *insertFunction(QString name,
                                        ValueTypeEnum type,
                                        SymbolsTable *params);

    StructTypeTableRecord *insertStructType(QString name,
                                    SymbolsTable *variables);

    StructTableRecord *insertStruct(QString name,
                                    StructTypeTableRecord *typeStruct);


    /**
     * @brief getVariable find variable into values list by name in current table
     * @param name searching variable name
     * @return pointer to struct of searching variable. If searching failed return NULL
     */
    AbstractSymbolTableRecord *getVariable(QString name);
    /**
     * @brief getVariableGlobal find variable into values list by name in current table and all parents
     * @param name searching variable name
     * @return pointer to struct of searching variable. If searching failed return NULL
     */
    AbstractSymbolTableRecord* getVariableGlobal(QString name);
    /**
     * @brief containsGlobal find variable into values list by name in current table and all parents
     * @param name searching variable name
     * @return true if variable found or false if variable not founded
     */

    bool containsGlobal(QString name);

    bool contains(QString name);

private:
    SymbolsTable *parent;
    QList<SymbolsTable*> childs;
    bool _isHidden;
};

#endif // SYMBOLSTABLE_H
