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
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Declaration function: %1, type: %2")
                 .arg(_variable->getName())
                 .arg(typeName.at(_variable->getValueType())).toStdString()
              << std::endl;

    if (_paramList != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Params:"
                  << std::endl;
        _paramList->printNode(level+1);
    }

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Body:"
              << std::endl;
    _body->printNode(level+1);
}

FunctionDeclareNode::~FunctionDeclareNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
    _body->~AbstractASTNode();
}


