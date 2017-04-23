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

QString ReferenceNode::printTripleCode(int level, QString param)
{
    return QString("%1%2")
            .arg(_variable->isGlobal() ? "@" : "%")
            .arg(_variable->getName());
}

ReferenceNode::~ReferenceNode()
{

}

