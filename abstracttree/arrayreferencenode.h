#ifndef REFERENCEARRAYNODE_H
#define REFERENCEARRAYNODE_H

#include "abstracttree/abstractvalueastnode.h"

/**
 * @brief The ReferenceNode class Reference to variabel
 */
class ArrayReferenceNode : public AbstractValueASTNode
{
public:
    ArrayReferenceNode(AbstractSymbolTableRecord *array, AbstractValueASTNode *index);

    void printNode(int level);
    AbstractValueASTNode* getIndex(){
        return _index;
    }
    AbstractSymbolTableRecord* getId(){
        return _array;
    }
    ~ArrayReferenceNode();

private:
    AbstractSymbolTableRecord *_array;
    AbstractValueASTNode *_index;
};

#endif // REFERENCEARRAYNODE_H
