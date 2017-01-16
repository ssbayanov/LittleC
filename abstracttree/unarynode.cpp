#include "unarynode.h"

UnaryNode::UnaryNode(TypeUnary uType, AbstractASTNode *left)
    : AbstractValueASTNode(NT_UnaryOperation)
{
    _uType = uType;
    _left = left;
    switch (_uType) {
    case UnarToInt:
        _typeValue = typeInt;
        break;
    case UnarToDouble:
        _typeValue = typeDouble;
        break;
    case UnarToBool:
        _typeValue = typeBool;
        break;
    default:
        _typeValue = ((AbstractValueASTNode *)_left)->getType();
    }
}

ValueTypeEnum UnaryNode::getType()
{
    return _typeValue;
}

void UnaryNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Unar operation. Type: %1. Type of result: %2")
                 .arg(unarTypeString.at(_uType))
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Left:"
              << std::endl;
    _left->printNode(level+1);

}

UnaryNode::~UnaryNode()
{
    _left->~AbstractASTNode();
}
