#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "abstracttree/abstractastnode.h"
#include "abstracttree/abstractvalueastnode.h"
#include "valuenode.h"


class AssignmentNode : public AbstractASTNode
{
public:
    AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value, AbstractASTNode* index);

    void printNode(int level);

    QString printTripleCode();

private:
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
    int _index;
};

#endif // ASSIGNMENTNODE_H
