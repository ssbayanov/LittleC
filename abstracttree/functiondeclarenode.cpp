#include "functiondeclarenode.h"

FunctionDeclareNode::FunctionDeclareNode(AbstractSymbolTableRecord *variable, AbstractASTNode *paramList, AbstractASTNode *body)
    : AbstractValueASTNode(NT_FunctionDeclare)
{
    _variable = variable;
    _paramList = paramList;
    _body = body;
}

void FunctionDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Declaration function: %1, type: %2\n")
                 .arg(_variable->getName())
                 .arg(typeName.at(_variable->getValueType()));

    if (_paramList != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Params:\n";
        _paramList->printNode(level+1);
    }

    if (_body != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Body:\n";
        _body->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD BODY NODE!!!\n";
    }
}

QString FunctionDeclareNode::printTripleCode(int level, QString param)
{
    outStream << QString("Function_%1:\n").arg(_variable->getName());
    if(_body != NULL){
        _body->printTripleCode();
    }

    //outStream << QString("LoopEnd_%1:\n").arg(_key);

    return "";
}

FunctionDeclareNode::~FunctionDeclareNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
    if (_body != NULL)
        _body->~AbstractASTNode();
}


