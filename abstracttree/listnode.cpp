#include "listnode.h"

ListNode::ListNode(AbstractASTNode *left, AbstractASTNode *right)
    : AbstractASTNode(NT_List)
{
    _left = left;
    _right = right;
}

void ListNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "List:"
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Left:"
              << std::endl;

    if(_left != NULL)
        _left->printNode(level+1);
    else
        std::cout << "BAD LEFT LIST NODE";

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Right:"
              << std::endl;

    if(_right != NULL)
        _right->printNode(level+1);
    else
        std::cout << "BAD RIGHT LIST NODE";

}

ListNode::~ListNode()
{
    _left->~AbstractASTNode();
    _right->~AbstractASTNode();
}

