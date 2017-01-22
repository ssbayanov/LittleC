#include "valuenode.h"

ValueNode::ValueNode(ValueTypeEnum typeValue, QVariant value)
    : AbstractValueASTNode(NT_NumericConstant)
{
    _value = value;
    _typeValue = typeValue;
}

void ValueNode::printNode(int level)
{

    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Value: %1, type: %2").arg(_value.toString())
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;
}

ValueTypeEnum ValueNode::getValueType()
{
    return _typeValue;
}

ValueNode::~ValueNode()
{

}

