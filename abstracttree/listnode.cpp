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
    if(_left != NULL){
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Left:"
                  << std::endl;
        _left->printNode(level+1);
    }
    else {
        std::cout  << QString().fill(' ',level*2).toStdString()
                   << "BAD LEFT LIST NODE"
                   << std::endl;
    }

    if(_right != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Right:"
                  << std::endl;
        _right->printNode(level+1);
    }
    else
        std::cout << "BAD RIGHT LIST NODE";

}

ListNode::~ListNode()
{
    if(_left != NULL)
        _left->~AbstractASTNode();
    if(_right != NULL)
        _right->~AbstractASTNode();
}

