#include "binarnode.h"

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
    AbstractValueASTNode::_typeValue = ((AbstractValueASTNode *)left)->getValueType();
}

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
    AbstractValueASTNode::_typeValue = typeValue;
}

void BinarNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Binar operation: %1, type: %2\n")
                 .arg(_operation)
                 .arg(typeName.at(getValueType()));

    if (_left != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Left:\n";
        _left->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD LEFT NODE!!!\n";
    }

    if (_right != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Right:\n";
        _right->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD RIGHT NODE!!!\n";
    }
}

BinarNode::~BinarNode()
{
    if (_left != NULL)
        _left->~AbstractASTNode();
    if (_right != NULL)
        _right->~AbstractASTNode();
}

