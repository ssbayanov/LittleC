#include "printnode.h"

PrintNode::PrintNode(AbstractASTNode *expression)
    : AbstractASTNode(NT_PintStatement)
{
    _expression = expression;
}

void PrintNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Print:"
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Expression:"
              << std::endl;
    _expression->printNode(level+1);
}

PrintNode::~PrintNode()
{
    _expression->~AbstractASTNode();
}


