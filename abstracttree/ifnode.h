#ifndef IFNODE_H
#define IFNODE_H

#include "abstracttree/abstractastnode.h"

class IfNode : public AbstractASTNode
{
public:
    IfNode(AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

    ~IfNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_trueBranch;
    AbstractASTNode *_falseBranch;
};


#endif // IFNODE_H
