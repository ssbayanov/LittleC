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

    if (_value != NULL) {
        _value->printNode(level+1);
    }
}

QString VariableDeclarationNode::printTripleCode()
{

    if(_variable->isGlobal()) {
        ir.writeLine( QString("@%1 = global %2 %3")
                      .arg(_variable->getName())
                      .arg(getValueTypeLLVM())
                      .arg((_value == NULL) ? "": _value->printTripleCode()));
    }
    else {
        ir.writeLine( QString("%%1 = alloca %2")
                      .arg(_variable->getName())
                      .arg(getValueTypeLLVM()));
        if(_value != NULL) {
            ir.writeLine( QString("store %1 %2, %1* %%3")
                          .arg(getValueTypeLLVM())
                          .arg(_value->printTripleCode())
                          .arg(_variable->getName()));
        }
    }

    return "";
}

QString VariableDeclarationNode::printParamTriple()
{
    QString outString = QString("%1 %_value_of_param_%2")
            .arg(getValueTypeLLVM())
            .arg(_variable->getName());

    ir.writeLine(QString("%%2 = alloca %1")
                 .arg(getValueTypeLLVM())
                 .arg(_variable->getName()));

    ir.writeLine(QString("store %1 %_value_of_param_%2, %1* %%2")
                 .arg(getValueTypeLLVM())
                 .arg(_variable->getName()));

    return outString;
}

VariableDeclarationNode::~VariableDeclarationNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}
