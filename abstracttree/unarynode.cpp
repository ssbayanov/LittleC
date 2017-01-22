#include "unarynode.h"

UnaryNode::UnaryNode(TypeUnary uType, AbstractASTNode *left)
    : AbstractValueASTNode(NT_UnaryOperation)
{
    _uType = uType;
    _value = left;
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
        _typeValue = ((AbstractValueASTNode *)_value)->getValueType();
    }
}

ValueTypeEnum UnaryNode::getValueType()
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

    if(_value != NULL) {
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
}

UnaryNode::~UnaryNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}
