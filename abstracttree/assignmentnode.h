#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "abstracttree/abstractastnode.h"
#include "abstracttree/abstractvalueastnode.h"

class AssignmentNode : public AbstractASTNode
{
public:
    AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

private:
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
};

#endif // ASSIGNMENTNODE_H
