#include "printnode.h"

PrintNode::PrintNode(AbstractASTNode *expression)
    : AbstractASTNode(NT_PintStatement)
{
    _expression = expression;
}

void PrintNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << "Print:\n";
    if (_expression != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Expression:\n";
        _expression->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD EXPRESSION NODE!!!\n";
    }
}

QString PrintNode::printTripleCode(int level, QString param)
{
    return "";
}

PrintNode::~PrintNode()
{
    if (_expression != NULL)
        _expression->~AbstractASTNode();
}


