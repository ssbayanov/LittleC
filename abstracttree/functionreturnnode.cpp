#include "functionreturnnode.h"

FunctionReturnNode::FunctionReturnNode(AbstractASTNode *expression)
    : AbstractValueASTNode(NT_FunctionReturn)
{
    _expression = expression;
    _typeValue = ((AbstractValueASTNode *)expression)->getType();
}

void FunctionReturnNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Return type %1")
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Expression:"
              << std::endl;
    _expression->printNode(level+1);

}

FunctionReturnNode::~FunctionReturnNode()
{
    _expression->~AbstractASTNode();
}
