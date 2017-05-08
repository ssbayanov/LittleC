#ifndef LISTNODE_H
#define LISTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The ListNode class Set of expressions
 */

typedef enum{
    LT_Default,
    LT_DeclareParamList,
    LT_CallParamList,
    LT_CaseList,
    LT_CaseValueList,
    LT_ArrayValueList
} ListTypeEnum;

class ListNode : public AbstractASTNode
{
public:
    ListNode(AbstractASTNode *left, AbstractASTNode *right, ListTypeEnum typeList = LT_Default);

    void printNode(int level);

    QString printTripleCode();

    AbstractASTNode *getLeftNode();
    AbstractASTNode *getRightNode();

    void setLeftNode(AbstractASTNode *left);
    void setRightNode(AbstractASTNode *right);

    void setListType(ListTypeEnum typeList);
    uint getSize();
    ~ListNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
    ListTypeEnum _typeList;
};

#endif // LISTNODE_H
