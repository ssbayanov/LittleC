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

    QString printTripleCode(int level, QString param);

    ~PrintNode();

private:
    AbstractASTNode *_expression;

    QString printTextVariable(QString text);
};


#endif // PRINTNODE_H
