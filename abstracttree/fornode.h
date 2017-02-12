#ifndef FORNODE_H
#define FORNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The ForNode class
 */
class ForNode : public AbstractASTNode
{
public:
    ForNode(AbstractASTNode *init, AbstractASTNode *condition, AbstractASTNode *increment, AbstractASTNode *body = NULL);

    void setBody(AbstractASTNode *body);

    void printNode(int level);

    ~ForNode();

private:
    AbstractASTNode *_init;
    AbstractASTNode *_condition;
    AbstractASTNode *_increment;
    AbstractASTNode *_body;
};



#endif // FORNODE_H
