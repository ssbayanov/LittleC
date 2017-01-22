#include "fornode.h"

ForNode::ForNode(AbstractASTNode *init, AbstractASTNode *condition, AbstractASTNode *increment, AbstractASTNode *body)
    : AbstractASTNode(NT_ForStatement)
{
    _init = init;
    _condition = condition;
    _increment = increment;
    _body = body;
}

void ForNode::setBody(AbstractASTNode *body)
{
    _body = body;
}

void ForNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "For:"
              << std::endl;
    if(_init != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Initialisation:"
                  << std::endl;
        _init->printNode(level+1);
    }

    if(_condition != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Condition:"
                  << std::endl;
        _condition->printNode(level+1);
    }

    if(_increment != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Increment:"
                  << std::endl;
        _increment->printNode(level+1);
    }

    if(_body != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Body:"
                  << std::endl;
        _body->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD BODY NODE!!!"
                  << std::endl;
    }
}

ForNode::~ForNode()
{
    if(_init != NULL)
        _init->~AbstractASTNode();
    if(_condition != NULL)
        _condition->~AbstractASTNode();
    if(_increment != NULL)
        _increment->~AbstractASTNode();
    if(_body != NULL)
        _body->~AbstractASTNode();
}

