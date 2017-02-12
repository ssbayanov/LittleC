#ifndef IFNODE_H
#define IFNODE_H

#include "abstracttree/abstractastnode.h"

class IfNode : public AbstractASTNode
{
public:
    IfNode(AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch);

    void printNode(int level);
    AbstractASTNode * getCond(){
        return _condition;
    }
    AbstractASTNode * getTrueBr(){
        return _trueBranch;
    }
    AbstractASTNode * getFalseBr(){
        return _falseBranch;
    }

    ~IfNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_trueBranch;
    AbstractASTNode *_falseBranch;
};


#endif // IFNODE_H
