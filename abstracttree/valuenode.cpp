#include "valuenode.h"
#include <QDebug>
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

QString ValueNode::printTripleCode()
{
    if(getValueType() == typeString) {
        QString value = _value.toString();
        QString result = "";
        int i;
        for(i = 0; i < _value.toByteArray().length(); i++) {
            wchar_t c = value.toStdString().at(i);
            if(c > 0x00ff || c < 0x000f){

                result.append(QString("\\%1")
                              .arg(value.toStdString().at(i), 2 , 16, QChar('0')));
            }
            else
                result.append(c);
        }
        result.replace("ffffffffffffff","");
        return ir.addUnnamedVariable(
                    QString("private unnamed_addr constant [%1 x i8] c\"%2\\00\"")
                    .arg(_value.toByteArray().length()+1)
                    .arg(result
                         /*.replace("\n","\\0A")
                                                           .replace("\t", "\\09")*/));
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
