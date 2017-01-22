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
    if (_expression != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Expression:"
                  << std::endl;
        _expression->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD EXPRESSION NODE!!!"
                  << std::endl;
    }
}

PrintNode::~PrintNode()
{
    if (_expression != NULL)
        _expression->~AbstractASTNode();
}


