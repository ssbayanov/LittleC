#include "functioncallnode.h"

FunctionCallNode::FunctionCallNode(AbstractSymbolTableRecord *variable, AbstractASTNode *paramList)
    : AbstractValueASTNode(NT_FunctionCall)
{
    _variable = variable;
    _paramList = paramList;
    _typeValue = _variable->getValueType();
}

void FunctionCallNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Call function: %1, type: %2")
                 .arg(_variable->getName())
                 .arg(typeName.at(_variable->getValueType())).toStdString()
              << std::endl;

    if (_paramList != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Params:"
                  << std::endl;
        _paramList->printNode(level+1);
    }
}

FunctionCallNode::~FunctionCallNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
}
