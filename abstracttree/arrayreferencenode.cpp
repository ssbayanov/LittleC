#include "arrayreferencenode.h"

ArrayReferenceNode::ArrayReferenceNode(AbstractSymbolTableRecord *array, AbstractValueASTNode *index)
    : AbstractValueASTNode(NT_ArrayReference)
{
    _array = array;
    _index = index;
    _typeValue = array->getValueType();
}

void ArrayReferenceNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Array Reference: %1, type: %2\n").arg(_array->getName()).arg(typeName.at(getValueType()));

    if (_index != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Index:\n";
        _index->printNode(level + 1);
    }
    else {
        treeStream << "BAD INDEX NODE!!!";
    }
}

ArrayReferenceNode::~ArrayReferenceNode()
{
    if (_index != NULL) {
        _index->~AbstractASTNode();
    }
}

