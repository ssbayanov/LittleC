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

QString FunctionCallNode::printTripleCode(int level, QString param)
{
    int numParams = 0;
    if(_paramList != NULL) {
        numParams = _paramList->printTripleCode(level+1,"isParamList").toInt();
    }

    outStream << QString("\tcall %1, %2\n")
                 .arg(_variable->getName())
                 .arg(numParams);

    return "";
}

FunctionCallNode::~FunctionCallNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
}
