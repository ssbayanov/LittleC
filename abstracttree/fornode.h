#ifndef FORNODE_H
#define FORNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The ForNode class
 */
class ForNode : public AbstractASTNode
{
public:
    ForNode(QString key, AbstractASTNode *init, AbstractASTNode *condition, AbstractASTNode *increment, AbstractASTNode *body = NULL);

    void setBody(AbstractASTNode *body);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

    ~ForNode();

private:
    AbstractASTNode *_init;
    AbstractASTNode *_condition;
    AbstractASTNode *_iteration;
    AbstractASTNode *_body;
    QString _key;
};



#endif // FORNODE_H
