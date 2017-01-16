#ifndef ARRAYDECLARENODE_H
#define ARRAYDECLARENODE_H

#include "abstracttree/abstractastnode.h"

class ArrayDeclareNode : public AbstractASTNode
{
public:
    ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values);

    void printNode(int level);

    ~ArrayDeclareNode();

private:
    AbstractSymbolTableRecord *_array;
    AbstractASTNode *_values;
};

#endif // ARRAYDECLARENODE_H
