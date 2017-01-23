#include "referencenode.h"

ReferenceNode::ReferenceNode(AbstractSymbolTableRecord *variable)
    : AbstractValueASTNode(NT_Reference)
{
    _variable = variable;
    _typeValue = _variable->getValueType();
}

void ReferenceNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Reference: %1, type: %2\n")
                 .arg(_variable->getName())
                 .arg(typeName.at(_typeValue));
}

ValueTypeEnum ReferenceNode::getValueType()
{
    return _typeValue;
}

ReferenceNode::~ReferenceNode()
{

}

