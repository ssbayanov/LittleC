#include "valuenode.h"

ValueNode::ValueNode(ValueTypeEnum typeValue, QVariant value)
    : AbstractValueASTNode(NT_NumericConstant)
{
    _value = value;
    _typeValue = typeValue;
}

void ValueNode::printNode(int level)
{

    treeStream << QString().fill(' ',level*2)
              << QString("Value: %1, type: %2\n").arg(_value.toString())
                 .arg(typeName.at(_typeValue));
}

ValueNode::~ValueNode()
{

}

