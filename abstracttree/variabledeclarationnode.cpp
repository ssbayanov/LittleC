#include "variabledeclarationnode.h"

VariableDeclarationNode::VariableDeclarationNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value)
    :AbstractValueASTNode(NT_Declaration)
{
    _variable = variable;
    _value = value;
    _typeValue = variable->getValueType();
}

void VariableDeclarationNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Declaration variable: %1, type: %2\n")
                 .arg(_variable->getName())
                 .arg(_variable->getTypeName());
}

QString VariableDeclarationNode::printTripleCode(int level, QString param)
{
    outStream << QString("%1%2 = external global %3 %4\n")
                 .arg(_variable->isGlobal() ? "@" : "%")
                 .arg(_variable->getName())
                 .arg(getValueTypeLLVM())
                 .arg((_value == NULL) ? "": _value->printTripleCode(level));

    return "";
}

VariableDeclarationNode::~VariableDeclarationNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}
