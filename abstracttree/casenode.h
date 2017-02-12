#ifndef CASENODE_H
#define CASENODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The CaseNode class
 */
class CaseNode : public AbstractASTNode
{
public:
    CaseNode(AbstractASTNode *value, AbstractASTNode *expressions);

    void printNode(int level);

    ~CaseNode();

private:
    AbstractASTNode *_value;
    AbstractASTNode *_expressions;
};

#endif // CASENODE_H
