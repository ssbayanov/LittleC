#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The AssignmentNode class
 */
class AssignmentNode : public AbstractASTNode
{
public:
    AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value);

    void printNode(int level);

    ~AssignmentNode();

private:
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
};


#endif // ASSIGNMENTNODE_H
