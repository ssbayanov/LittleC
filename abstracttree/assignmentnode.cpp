#include "assignmentnode.h"

AssignmentNode::AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value)
    : AbstractASTNode(NT_AssignmentOperation)
{
    _variable = variable;
    _value = value;
}

void AssignmentNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Assignment: %1\n").arg(_variable->getName());

    if (_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Value:\n";
        _value->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD VALUE NODE!!!\n";
    }
}

AssignmentNode::~AssignmentNode()
{
    _value->~AbstractASTNode();
}
