#include "assignmentnode.h"

AssignmentNode::AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value)
    : AbstractASTNode(NT_AssignmentOperation)
{
    _variable = variable;
    _value = value;
}

void AssignmentNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Assignment: %1").arg(_variable->getName()).toStdString()
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Value:"
              << std::endl;
    _value->printNode(level+1);
}

AssignmentNode::~AssignmentNode()
{
    _value->~AbstractASTNode();
}
