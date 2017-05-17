#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "abstracttree/abstractvalueastnode.h"

typedef enum
{
    UnarToDouble,
    UnarToInt,
    UnarToBool,
    UnarToChar,
    UnarToFloat,
    UnarToShort,
    UnarMinus,
    UnarNot,
    UnarIncrement,
    UnarPreincrement,
    UnarDecrement,
    UnarPredecrement
}TypeUnary;

static QStringList unarTypeString = QStringList() << "To double"
                                              << "To integer"
                                              << "To bool"
                                              << "To char"
                                              << "To float"
                                              << "To short"
                                              << "Unar \"-\""
                                              << "Bool Not";

static QStringList unarOperationCommand = QStringList() << "unartodouble"
                                              << "unartointeger"
                                              << "unartobool"
                                              << "unartochar"
                                              << "unartofloat"
                                              << "unartoshort"
                                              << "unarminus"
                                              << "unarnot";

/**
 * @brief The UnaryNode class node for storage unary operations
 */
class UnaryNode : public AbstractValueASTNode
{
public:
    UnaryNode(TypeUnary uType, AbstractASTNode *left);

    UnaryNode(TypeUnary uType, AbstractSymbolTableRecord *variable);

    void printNode(int level);

    virtual QString printTripleCode();

    ~UnaryNode();

private:
    AbstractASTNode *_value;
    AbstractSymbolTableRecord *_variable;

    TypeUnary _uType;

    QString writeIncrement(bool isPreincrement = false, bool isDecrement = false);
};


#endif // UNARYNODE_H
