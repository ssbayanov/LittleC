#ifndef CASENODE_H
#define CASENODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The CaseNode class
 */
class CaseNode : public AbstractASTNode
{
public:
    CaseNode(QString key, AbstractASTNode *value, AbstractASTNode *statements);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

    ~CaseNode();

private:
    AbstractASTNode *_value;
    AbstractASTNode *_statements;
    QString _key;
};

#endif // CASENODE_H
