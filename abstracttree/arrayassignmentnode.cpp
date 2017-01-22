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
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Array Assignment: %1").arg(_array->getName()).toStdString()
              << std::endl;

    if (_value != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Value:"
                  << std::endl;
        _value->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD VALUE NODE!!!"
                  << std::endl;
    }

    if (_index != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Index:"
                  << std::endl;
        _index->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD INDEX NODE!!!"
                  << std::endl;
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
