#include "functionreturnnode.h"

FunctionReturnNode::FunctionReturnNode(AbstractASTNode *expression)
    : AbstractValueASTNode(NT_FunctionReturn)
{
    _expression = expression;
    _typeValue = ((AbstractValueASTNode *)expression)->getValueType();
}

void FunctionReturnNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Return type %1\n")
                  .arg(typeName.at(_typeValue));

    treeStream << QString().fill(' ',level*2)
               << "Expression:\n";
    _expression->printNode(level+1);

}

FunctionReturnNode::~FunctionReturnNode()
{
    _expression->~AbstractASTNode();
}
