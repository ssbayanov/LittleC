#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "abstracttree/abstractastnode.h"
#include "abstracttree/abstractvalueastnode.h"

class AssignmentNode : public AbstractASTNode
{
public:
    AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value);

    /**
     * @brief printNode Virtual function of printing tree. Tree printed to treeStream.
     * @param level Level of node. Setup of space.
     */
    void printNode(int level);

    QString printTripleCode();

private:
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
};

#endif // ASSIGNMENTNODE_H
