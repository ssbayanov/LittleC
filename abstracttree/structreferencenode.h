#ifndef STRUCTREFERENCENODE_H
#define STRUCTREFERENCENODE_H

#include "abstracttree/abstractvalueastnode.h"
#include "abstractvalueastnode.h"

class StructReferenceNode : public AbstractValueASTNode
{
public:
    StructReferenceNode(AbstractSymbolTableRecord *structVariable, AbstractSymbolTableRecord *variable);

    void printNode(int level);

    QString printTripleCode();

    ~StructReferenceNode();

private:
    AbstractSymbolTableRecord *_structVariable;
    AbstractSymbolTableRecord *_variable;
};

#endif // STRUCTREFERENCENODE_H
