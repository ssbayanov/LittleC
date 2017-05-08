#ifndef ARRAYDECLARENODE_H
#define ARRAYDECLARENODE_H

#include "abstracttree/abstractastnode.h"
#include "abstracttree/abstractvalueastnode.h"
#include "abstracttree/valuenode.h"
#include "abstracttree/listnode.h"
#include "symbolstable/arraytablerecord.h"

class ArrayDeclareNode : public AbstractASTNode
{
public:
    ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values);
    void printNode(int level);
    QString printTripleCode();
    ~ArrayDeclareNode();

private:
    AbstractSymbolTableRecord *_array;
    AbstractASTNode *_values;
};

#endif // ARRAYDECLARENODE_H
