#ifndef PRINTNODE_H
#define PRINTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The PrintNode class
 */
class PrintNode : public AbstractASTNode
{
public:
    PrintNode(AbstractASTNode *expression);

    void printNode(int level);

    ~PrintNode();

private:
    AbstractASTNode *_expression;
};


#endif // PRINTNODE_H
