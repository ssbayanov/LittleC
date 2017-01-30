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
    UnarNot
}TypeUnary;

static QStringList unarTypeString = QStringList() << "To double"
                                              << "To integer"
                                              << "To bool"
                                              << "Unar \"-\""
                                              << "Bool Not";

/**
 * @brief The UnaryNode class node for storage unary operations
 */
class UnaryNode : public AbstractValueASTNode
{
public:
    UnaryNode(TypeUnary uType, AbstractASTNode *left);

    void printNode(int level);

    ~UnaryNode();

private:
    AbstractASTNode *_value;
    TypeUnary _uType;
};


#endif // UNARYNODE_H
