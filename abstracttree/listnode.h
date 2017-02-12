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

    ~ListNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
};

#endif // LISTNODE_H
