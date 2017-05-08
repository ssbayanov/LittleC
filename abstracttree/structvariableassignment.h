#ifndef STRUCTVARIABLEASSIGNMENT_H
#define STRUCTVARIABLEASSIGNMENT_H

#include "abstracttree/abstractvalueastnode.h"


class StructVariableAssignment : public AbstractValueASTNode
{
public:
    StructVariableAssignment(AbstractSymbolTableRecord *structVariable, AbstractSymbolTableRecord *variable,
                             AbstractASTNode *value);

    void printNode(int level);

    QString printTripleCode();

private:
    AbstractSymbolTableRecord *_structVariable;
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
};

#endif // STRUCTVARIABLEASSIGNMENT_H
