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
    treeStream << QString().fill(' ',level*2)
               << QString("Call function: %1, type: %2\n")
                  .arg(_variable->getName())
                  .arg(typeName.at(_variable->getValueType()));

    if (_paramList != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Params:\n";
        _paramList->printNode(level+1);
    }
}

QString FunctionCallNode::printTripleCode()
{

    ir.writeCommandLine(QString("call %3 @%1(%2)\n")
                        .arg(_variable->getName())
                        .arg((_paramList != NULL) ?
                            _paramList->printTripleCode() : "")
                        .arg(getValueTypeLLVM()));

    return ir.lastCommandLine();
}

FunctionCallNode::~FunctionCallNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
}
