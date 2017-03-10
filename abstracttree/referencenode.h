#ifndef REFERENCENODE_H
#define REFERENCENODE_H

#include "abstracttree/abstractvalueastnode.h"

/**
 * @brief The ReferenceNode class Reference to variabel
 */
class ReferenceNode : public AbstractValueASTNode
{
public:
    ReferenceNode(AbstractSymbolTableRecord *variable);

    void printNode(int level);

    virtual QString printTripleCode(int level, QString param);

    ~ReferenceNode();

private:
    AbstractSymbolTableRecord *_variable;
};

#endif // REFERENCENODE_H
