#include "binarnode.h"

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
    _typeValue = ((AbstractValueASTNode *)left)->getType();
}

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
    _typeValue = typeValue;
}

void BinarNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Binar operation: %1, type: %2")
                 .arg(_operation)
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Left:"
              << std::endl;
    _left->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Right:"
              << std::endl;
    _right->printNode(level+1);
}

ValueTypeEnum BinarNode::getType()
{
    return typeDouble;
}

BinarNode::~BinarNode()
{
    _left->~AbstractASTNode();
    _right->~AbstractASTNode();
}

