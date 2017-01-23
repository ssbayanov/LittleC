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
    treeStream << QString().fill(' ',level*2)
              << (_isDoWhile == false ? "While:" : "Do-While")
              << "\n";


    if(!_isDoWhile && _condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Condition:\n";
        _condition->printNode(level+1);
    }

    if(_body != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Body:\n";
        _body->printNode(level+1);
    }

    if(_isDoWhile && _condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Condition:\n";
        _condition->printNode(level+1);
    }

    if (_condition == NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "BAD CONDITION NODE!!!\n";
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
    if(_body != NULL)
        _body->~AbstractASTNode();
}

