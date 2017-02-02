#ifndef STRUCTDECLARENODE_H
#define STRUCTDECLARENODE_H

#include "abstracttree/abstractvalueastnode.h"

class StructDeclareNode : public AbstractValueASTNode
{
public:
    StructDeclareNode(AbstractSymbolTableRecord *variable, AbstractASTNode *variablesList);

    void printNode(int level);

    ~StructDeclareNode();

private:
    AbstractASTNode *_variablesList;
    AbstractSymbolTableRecord *_variable;

};

#endif // STRUCTDECLARENODE_H
