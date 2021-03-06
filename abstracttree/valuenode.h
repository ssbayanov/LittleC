#ifndef VALUENODE_H
#define VALUENODE_H

#include "abstracttree/abstractvalueastnode.h"


class ValueNode : public AbstractValueASTNode
{
public:
    ValueNode(ValueTypeEnum typeValue, QVariant value);

    void printNode(int level);

    virtual QString printTripleCode();

    QVariant getValue();

    ~ValueNode();

private:
    QVariant _value;
};

#endif // VALUENODE_H
