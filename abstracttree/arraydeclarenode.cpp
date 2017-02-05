#include "arraydeclarenode.h"

ArrayDeclareNode::ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values)
    : AbstractASTNode(NT_ArrayDeclare)
{
    _array = array;
    _values = values;
}

void ArrayDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Array Declare: %1, type: %2\n")
                 .arg(_array->getName())
                 .arg(typeName.at(_array->getValueType()));

    if (_values != NULL) {
        treeStream
                << QString().fill(' ',level*2)
                << "Values:\n";
        _values->printNode(level + 1);
    }
    else {
        treeStream << "BAD VALUES NODE!!!\n";
    }
}

ArrayDeclareNode::~ArrayDeclareNode()
{
    if (_values != NULL) {
        _values->~AbstractASTNode();
    }
}
