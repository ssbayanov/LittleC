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
    treeStream << QString().fill(' ',level*2)
              << "For:\n";
    if(_init != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Initialisation:\n";
        _init->printNode(level+1);
    }

    if(_condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Condition:\n";
        _condition->printNode(level+1);
    }

    if(_increment != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Increment:\n";
        _increment->printNode(level+1);
    }

    if(_body != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Body:\n";
        _body->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD BODY NODE!!!\n";
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

