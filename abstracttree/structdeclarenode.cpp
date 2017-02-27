#include "structdeclarenode.h"

StructDeclareNode::StructDeclareNode(AbstractSymbolTableRecord *variable, AbstractASTNode *variablesList)
    : AbstractValueASTNode(NT_StructTypeDeclare)
{
    _variablesList = variablesList;
    _variable = variable;
}

void StructDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Declaration struct type: %1\n")
                 .arg(_variable->getName());

    if (_variablesList != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Variables:\n";
        _variablesList->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD VARIABLE LIST NODE!!!\n";
    }
}

StructDeclareNode::~StructDeclareNode()
{
    if (_variablesList != NULL)
        _variablesList->~AbstractASTNode();
}

