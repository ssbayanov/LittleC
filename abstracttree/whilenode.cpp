#include "whilenode.h"

WhileNode::WhileNode(AbstractASTNode *condition, AbstractASTNode *body)
    : AbstractASTNode(NT_WhileStatement)
{
    _condition = condition;
    _body = body;
    _isDoWhile = false;
}

void WhileNode::setBody(AbstractASTNode *body)
{
    _body = body;
}

void WhileNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << (_isDoWhile == false ? "While:" : "Do-While")
              << std::endl;


    if(!_isDoWhile && _condition != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Condition:"
                  << std::endl;
        _condition->printNode(level+1);
    }

    if(_body != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Body:"
                  << std::endl;
        _body->printNode(level+1);
    }
    if(_isDoWhile && _condition != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Condition:"
                  << std::endl;
        _condition->printNode(level+1);
    }
}

void WhileNode::setIsDoWhile(bool isDoWhile)
{
    _isDoWhile = isDoWhile;
}

WhileNode::~WhileNode()
{
    if(_condition != NULL)
        _condition->~AbstractASTNode();
    _body->~AbstractASTNode();
}

