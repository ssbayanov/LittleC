#ifndef IFNODE_H
#define IFNODE_H

#include "abstracttree/abstractastnode.h"

class IfNode : public AbstractASTNode
{
public:
    IfNode(QString key, AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

    ~IfNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_trueBranch;
    AbstractASTNode *_falseBranch;
    QString _key;
};


#endif // IFNODE_H
