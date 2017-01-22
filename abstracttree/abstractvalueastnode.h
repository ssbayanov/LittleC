#ifndef ABSTRACTVALUEASTNODE_H
#define ABSTRACTVALUEASTNODE_H

#include "abstracttree/abstractastnode.h"


class AbstractValueASTNode  : public AbstractASTNode
{
public:
    AbstractValueASTNode(ASTNodeTypeEnum type);

    virtual ValueTypeEnum getValueType();

    virtual ~AbstractValueASTNode();

    ValueTypeEnum _typeValue;
};
#endif // ABSTRACTVALUEASTNODE_H
