#ifndef ARRAYASSIGNMENTNODE_H
#define ARRAYASSIGNMENTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The ArrayAssignmentNode class Assignment new value of an array element
 */
class ArrayAssignmentNode : public AbstractASTNode
{
public:
    /**
     * @brief ArrayAssignmentNode Constructor of class.
     * @param array Pointer to array variable in symbol table.
     * @param index Pointer to node with index expression
     * @param value Pointer to new value of an array element
     */
    ArrayAssignmentNode(AbstractSymbolTableRecord *array, AbstractASTNode *index, AbstractASTNode *value);

    /**
     * @brief printNode Virtual function of printing tree. Tree printed to treeStream.
     * @param level Level of node. Setup of space.
     */
    void printNode(int level);

    QString printTripleCode();

private:
    AbstractSymbolTableRecord *_array;
    AbstractASTNode *_index;
    AbstractASTNode *_value;
};

#endif // ARRAYASSIGNMENTNODE_H
