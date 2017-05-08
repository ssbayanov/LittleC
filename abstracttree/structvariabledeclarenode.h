#ifndef STRUCTVARIABLEDECLARENODE_H
#define STRUCTVARIABLEDECLARENODE_H

#include "abstracttree/ast.h"
#include "abstractvalueastnode.h"

class StructVariableDeclareNode : public AbstractValueASTNode
{
public:
    StructVariableDeclareNode(AbstractSymbolTableRecord *typeStruct, AbstractSymbolTableRecord *variable);

    void printNode(int level);

    QString printTripleCode();

    AbstractSymbolTableRecord *getTypeStruct();
    AbstractSymbolTableRecord *getVariable();

    ~StructVariableDeclareNode();

private:
    AbstractSymbolTableRecord *_typeStruct;
    AbstractSymbolTableRecord *_variable;
};

#endif // STRUCTVARIABLEDECLARENODE_H
