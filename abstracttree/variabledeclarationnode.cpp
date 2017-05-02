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

    if(_value != NULL) {
        _value->printNode(level+1);
    }
}

QString VariableDeclarationNode::printTripleCode(int level, QString param)
{
    ir.writeLine( QString("%1%2 = %4 %3")
                 .arg(_variable->isGlobal() ? "@" : "%")
                 .arg(_variable->getName())
                 .arg((_value == NULL) ? "": _value->printTripleCode(level))
                 .arg(_variable->isGlobal() ? "global "+getValueTypeLLVM()
                                            : QString("add %1 0, ")
                                              .arg(getValueTypeLLVM())));



    return "";
}

VariableDeclarationNode::~VariableDeclarationNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}
