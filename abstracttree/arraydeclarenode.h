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
    ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values, AbstractASTNode *size);

    /**
     * @brief printNode Virtual function of printing tree. Tree printed to treeStream.
     * @param level Level of node. Setup of space.
     */
    void printNode(int level);
    QString printTripleCode();
    ~ArrayDeclareNode();

private:
    AbstractSymbolTableRecord *_array;
    AbstractASTNode *_values;
    AbstractASTNode *_size;

    void printAssign(uint index, AbstractASTNode *value);
    void printValues(uint index, AbstractASTNode *value);
};

#endif // ARRAYDECLARENODE_H
