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
    case UnarToChar:
        _typeValue = typeChar;
        break;
    case UnarToShort:
        _typeValue = typeShort;
        break;
    case UnarToFloat:
        _typeValue = typeFloat;
        break;
    default:
        _typeValue = ((AbstractValueASTNode *)_value)->getValueType();
    }
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

QString UnaryNode::printTripleCode(int level, QString)
{
    if(_value != NULL) {
        AbstractValueASTNode *value = ((AbstractValueASTNode *) _value);
        QString opText = "";
        switch (_uType) {
        case UnarToDouble:
            if(isInt(value->getValueType())) {
                ir.writeCommandLine( QString("sitofp %1 %2 to %3")
                        .arg(value->getValueTypeLLVM())
                        .arg(value->printTripleCode())
                        .arg(getValueTypeLLVM()));
            }

            break;
        case UnarToInt:
        case UnarToBool:
        case UnarToChar:
        case UnarToShort:
            if(isInt(value->getValueType())) {
                ir.writeCommandLine( QString("%1 %2 %3 to %4")
                        .arg(getSizeType(getValueType()) < getSizeType(value->getValueType()) ?
                                 "trunc" :
                                 "zext")
                        .arg(value->getValueTypeLLVM())
                        .arg(value->printTripleCode())
                        .arg(getValueTypeLLVM()));
            }
            else {
                ir.writeCommandLine( QString("fptosi %1 %2 to %3")
                        .arg(value->getValueTypeLLVM())
                        .arg(value->printTripleCode())
                        .arg(getValueTypeLLVM()));
            }
            break;
        case UnarToFloat:
            opText = "float ";
            break;
        case UnarMinus:
            ir.writeCommandLine( QString("sub %1 0, %2")
                                 .arg(value->getValueTypeLLVM())
                                 .arg(value->printTripleCode()));
            break;
        case UnarNot:
            opText = "!";
            break;
        default:
            break;
        }
        //outStream << opText;

        return QString(ir.lastCommandLine())
                .arg(level);

    }
    return "";
}

UnaryNode::~UnaryNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}
