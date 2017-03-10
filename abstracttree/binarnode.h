#ifndef BINARNODE_H
#define BINARNODE_H

#include "abstracttree/abstractvalueastnode.h"
/**
 * @brief The BinarNode class
 */
class BinarNode : public AbstractValueASTNode
{
public:
    BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation);

    BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue);

    virtual QString printTripleCode(int level, QString param);

    void printNode(int level);

    ~BinarNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
    QString _operation;
};


#endif // BINARNODE_H
