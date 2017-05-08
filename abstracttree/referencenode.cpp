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

QString ReferenceNode::printTripleCode()
{
    ir.writeCommandLine(QString("load %1, %1* %2%3")
                        .arg(getValueTypeLLVM())
                        .arg(_variable->isGlobal() ? "@" : "%")
                        .arg(_variable->getName()));

    return ir.lastCommandLine();

}

ReferenceNode::~ReferenceNode()
{

}

