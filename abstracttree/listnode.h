#ifndef LISTNODE_H
#define LISTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The ListNode class Set of expressions
 */
class ListNode : public AbstractASTNode
{
public:
    ListNode(AbstractASTNode *left, AbstractASTNode *right);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

    AbstractASTNode *getLeftNode();
    AbstractASTNode *getRightNode();

    void setLeftNode(AbstractASTNode *left);
    void setRightNode(AbstractASTNode *right);

    ~ListNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
};

#endif // LISTNODE_H
