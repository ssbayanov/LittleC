#include "structreferencenode.h"

StructReferenceNode::StructReferenceNode(AbstractSymbolTableRecord *structVariable, AbstractSymbolTableRecord *variable)
    : AbstractValueASTNode(NT_StructReference)
{
    _structVariable = structVariable;
    _variable = variable;
    _typeValue = variable->getValueType();
}

void StructReferenceNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Struct reference: %1.%2, type: %3\n")
                 .arg(_structVariable->getName())
                 .arg(_variable->getName())
                 .arg(typeName.at(_variable->getValueType()));
}

StructReferenceNode::~StructReferenceNode()
{

}
