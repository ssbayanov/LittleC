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
    std::cout << QString().fill(' ',level*2).toStdString()
              << "If"
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Condition:"
              << std::endl;
    _condition->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "True branch:"
              << std::endl;
    _trueBranch->printNode(level+1);
    if (_falseBranch != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "False branch:"
                  << std::endl;
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
