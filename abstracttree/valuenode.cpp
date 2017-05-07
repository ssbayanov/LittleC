#include "valuenode.h"

ValueNode::ValueNode(ValueTypeEnum typeValue, QVariant value)
    : AbstractValueASTNode(NT_NumericConstant)
{

    _typeValue = typeValue;
    if(_typeValue == typeString){
        _value = value.toString()
                .replace("\\t", "\t")
                .replace("\\n", "\n");
    }
    else {
    _value = value;
    }
}

void ValueNode::printNode(int level)
{

    treeStream << QString().fill(' ',level*2)
               << QString("Value: %1, type: %2\n").arg(_value.toString())
                  .arg(typeName.at(_typeValue));
}

QString ValueNode::printTripleCode(int level, QString param)
{
    if(getValueType() == typeString) {
        return ir.addUnnamedVariable(
                    QString("private unnamed_addr constant [%1 x i8] c\"%2\\00\"")
                    .arg(_value.toString().length()+1)
                    .arg(_value.toString()
                         .replace("\n","\\0A")
                         .replace("\t", "\\09")));
    }
    if(getValueType() == typeDouble || getValueType() == typeFloat)
        return QString("%1").arg(_value.toDouble(), 0, 'e', 6);

    return _value.toString();
}

QVariant ValueNode::getValue()
{
    return _value;
}

ValueNode::~ValueNode()
{

}
