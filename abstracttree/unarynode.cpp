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
    treeStream << QString().fill(' ',level*2)
              << QString("Unar operation. Type: %1. Type of result: %2\n")
                 .arg(unarTypeString.at(_uType))
                 .arg(typeName.at(_typeValue));

    if(_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Value:\n";
        _value->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD VALUE NODE!!!\n";
    }
}

UnaryNode::~UnaryNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}
