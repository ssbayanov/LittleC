#include "ifnode.h"

IfNode::IfNode(AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch)
    : AbstractASTNode(NT_IfStatement)
{
    _condition = condition;
    _trueBranch = trueBranch;
    _falseBranch = falseBranch;
}

void IfNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << "If\n";
    if (_condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Condition:\n";
        _condition->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD CONDITION NODE!!!\n";
    }

    if (_trueBranch != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "True branch:\n";
        _trueBranch->printNode(level+1);
    }
    if (_falseBranch != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "False branch:\n";
        _falseBranch->printNode(level+1);
    }
}

IfNode::~IfNode()
{
    _condition->~AbstractASTNode();
    _trueBranch->~AbstractASTNode();
    if (_falseBranch != NULL)
        _falseBranch->~AbstractASTNode();
}
