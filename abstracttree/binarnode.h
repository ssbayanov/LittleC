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

    void printNode(int level);

    ValueTypeEnum getType();
    AbstractASTNode * getLeft(){
        return _left;
    }
    AbstractASTNode * getRight(){
        return _right;
    }

    ~BinarNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
    QString _operation;
};


#endif // BINARNODE_H
