#ifndef CASENODE_H
#define CASENODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The CaseNode class
 */
class CaseNode : public AbstractASTNode
{
public:
    CaseNode(AbstractASTNode *value, AbstractASTNode *statements);

    void printNode(int level);

    QString printTripleCode();

    QString printValues();

    ~CaseNode();

private:
    AbstractASTNode *_value;
    AbstractASTNode *_statements;

    QString valuePtr;
};

#endif // CASENODE_H
