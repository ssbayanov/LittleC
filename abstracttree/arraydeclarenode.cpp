#include "arraydeclarenode.h"

ArrayDeclareNode::ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values)
    : AbstractASTNode(NT_ArrayDeclare)
{
    _array = array;
    _values = values;
}

void ArrayDeclareNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Array Declare: %1, type: %2").arg(_array->getName()).arg(typeName.at(_array->getValueType())).toStdString()
              << std::endl;

    if (_values != NULL) {
        std::cout << "Values:";
        _values->printNode(level + 1);
    }
    else {
        std::cout << "BAD VALUES NODE!!!";
    }
}

ArrayDeclareNode::~ArrayDeclareNode()
{
    if (_values != NULL) {
        _values->~AbstractASTNode();
    }
}
