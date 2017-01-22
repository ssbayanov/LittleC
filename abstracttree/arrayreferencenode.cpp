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
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Array Reference: %1, type: %2").arg(_array->getName()).arg(typeName.at(_typeValue)).toStdString()
              << std::endl;

    if (_index != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Index:"
                  << std::endl;
        _index->printNode(level + 1);
    }
    else {
        std::cout << "BAD INDEX NODE!!!";
    }
}

ValueTypeEnum ArrayReferenceNode::getValueType()
{
    return _typeValue;
}

ArrayReferenceNode::~ArrayReferenceNode()
{
    if (_index != NULL) {
        _index->~AbstractASTNode();
    }
}

