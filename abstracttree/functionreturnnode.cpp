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

QString FunctionReturnNode::printTripleCode(int level, QString param)
{
    outStream << QString("ret %2 %1\n")
                 .arg(_expression->printTripleCode(level+1, param))
                 .arg(((AbstractValueASTNode *) _expression)->getValueTypeLLVM());

    return "";

}

FunctionReturnNode::~FunctionReturnNode()
{
    _expression->~AbstractASTNode();
}
