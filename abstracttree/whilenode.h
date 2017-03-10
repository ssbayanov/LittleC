#ifndef WHILENODE_H
#define WHILENODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The WhileNode class
 */
class WhileNode : public AbstractASTNode
{
public:
    WhileNode(QString key, AbstractASTNode *condition, AbstractASTNode *body = NULL);

    void setBody(AbstractASTNode *body);

    void printNode(int level);

    QString printTripleCode(int level, QString param);

    void setIsDoWhile(bool isDoWhile = true);

    ~WhileNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_body;
    QString _key;
    bool _isDoWhile;
};


#endif // WHILENODE_H
