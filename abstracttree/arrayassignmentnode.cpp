#include "arrayassignmentnode.h"

ArrayAssignmentNode::ArrayAssignmentNode(AbstractSymbolTableRecord *array, AbstractASTNode *index, AbstractASTNode *value)
    : AbstractASTNode(NT_ArrayReference)
{
    _array = array;
    _index = index;
    _value = value;
}

void ArrayAssignmentNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Array Assignment: %1\n").arg(_array->getName());

    if (_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Value:\n";
        _value->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD VALUE NODE!!!\n";
    }

    if (_index != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Index:\n";
        _index->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD INDEX NODE!!!\n";
    }
}

ArrayAssignmentNode::~ArrayAssignmentNode()
{
    if (_value != NULL) {
        _value->~AbstractASTNode();
    }
    if (_index != NULL) {
        _index->~AbstractASTNode();
    }
}
