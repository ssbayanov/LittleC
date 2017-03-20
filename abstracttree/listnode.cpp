#include "listnode.h"

ListNode::ListNode(AbstractASTNode *left, AbstractASTNode *right)
    : AbstractASTNode(NT_List)
{
    _left = left;
    _right = right;
}

void ListNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << "List:\n";
    if(_left != NULL){
        treeStream << QString().fill(' ',level*2)
                   << "Left:\n";
        _left->printNode(level+1);
    }
    else {
        treeStream  << QString().fill(' ',level*2)
                    << "BAD LEFT LIST NODE\n";
        ;
    }

    if(_right != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Right:\n";
        _right->printNode(level+1);
    }
    else
        treeStream << "BAD RIGHT LIST NODE";

}

QString ListNode::printTripleCode(int level, QString param)
{
    if(_left != NULL)
        _left->printTripleCode(level+1, param);
    if(_right != NULL)
        _right->printTripleCode(level+2, param);
    return "";
}

AbstractASTNode *ListNode::getLeftNode()
{
    return _left;
}

AbstractASTNode *ListNode::getRightNode()
{
    return _right;
}

void ListNode::setLeftNode(AbstractASTNode *left)
{
    _left = left;
}

void ListNode::setRightNode(AbstractASTNode *right)
{
    _right = right;
}

ListNode::~ListNode()
{
    if(_left != NULL)
        _left->~AbstractASTNode();
    if(_right != NULL)
        _right->~AbstractASTNode();
}

