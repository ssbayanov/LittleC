#ifndef ARRAYASSIGNMENTNODE_H
#define ARRAYASSIGNMENTNODE_H
#include "abstracttree/abstractastnode.h"

/**
 * @brief The ArrayAssignmentNode class
 */
class ArrayAssignmentNode : public AbstractASTNode
{
public:
    ArrayAssignmentNode(AbstractSymbolTableRecord *array, AbstractASTNode *index, AbstractASTNode *value);

    void printNode(int level);

    ~ArrayAssignmentNode();

private:
    AbstractSymbolTableRecord *_array;
    AbstractASTNode *_index;
    AbstractASTNode *_value;
};

#endif // ARRAYASSIGNMENTNODE_H
