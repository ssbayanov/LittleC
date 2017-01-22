#include "referencenode.h"

ReferenceNode::ReferenceNode(AbstractSymbolTableRecord *variable)
    : AbstractValueASTNode(NT_Reference)
{
    _variable = variable;
    _typeValue = _variable->getValueType();
}

void ReferenceNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Reference: %1, type: %2").arg(_variable->getName()).arg(typeName.at(_typeValue)).toStdString()
              << std::endl;
}

ValueTypeEnum ReferenceNode::getValueType()
{
    return _typeValue;
}

ReferenceNode::~ReferenceNode()
{

}

