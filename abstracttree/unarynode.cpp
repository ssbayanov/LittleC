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
    _variable = NULL;
}

UnaryNode::UnaryNode(TypeUnary uType, AbstractSymbolTableRecord *variable)
    : AbstractValueASTNode(NT_UnaryOperation)
{
    _uType = uType;
    _typeValue = typeInt;
    _variable = variable;
    _value = NULL;
}

void UnaryNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Unar operation. Type: %1. Type of result: %2\n")
                  .arg(unarTypeString.at(_uType))
                  .arg(typeName.at(_typeValue));

    if(_value != NULL && _uType <= UnarNot) {
        treeStream << QString().fill(' ',level*2)
                   << "Value:\n";
        _value->printNode(level+1);
    }
    else {
        if(_variable != NULL && _uType >= UnarIncrement) {
            treeStream << QString().fill(' ',level*2)
                       << QString("Variable: %1\n").arg(_variable->getName());
        }
        else {
            treeStream << QString().fill(' ',level*2)
                       << "BAD VALUE NODE OR VARIABLE!!!\n";
        }
    }


}

QString UnaryNode::printTripleCode()
{
    if(_value != NULL || _variable != NULL) {
        AbstractValueASTNode *value = ((AbstractValueASTNode *) _value);

        switch (_uType) {
        case UnarToDouble:
            if(isInt(value->getValueType())) {
                ir.writeCommandLine( QString("sitofp %1 %2 to %3")
                                     .arg(value->getValueTypeLLVM())
                                     .arg(value->printTripleCode())
                                     .arg(getValueTypeLLVM()));
            }
            else {

                ir.writeCommandLine( QString("fpext float %1 to double")
                                     .arg(value->printTripleCode()));
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
            if(isInt(value->getValueType())) {
                ir.writeCommandLine( QString("sitofp %1 %2 to float")
                                     .arg(value->getValueTypeLLVM())
                                     .arg(value->printTripleCode()));

            }
            else {
                ir.writeCommandLine( QString("fptrunc double %1 to float")
                                     .arg(value->printTripleCode()));
            }
            break;
        case UnarMinus:
            ir.writeCommandLine( QString("sub %1 0, %2")
                                 .arg(value->getValueTypeLLVM())
                                 .arg(value->printTripleCode()));
            break;
        case UnarNot:
            ir.writeCommandLine( QString("sub %1 1, %2")
                                 .arg(value->getValueTypeLLVM())
                                 .arg(value->printTripleCode()));
            break;
        case UnarIncrement:
            return writeIncrement();
        case UnarPreincrement:
            return writeIncrement(true);
        case UnarDecrement:
            return writeIncrement(false, true);
        case UnarPredecrement:
            return writeIncrement(true, true);

        default:
            break;
        }

        return ir.lastCommandLine();

    }
    return "";
}

UnaryNode::~UnaryNode()
{
    if(_value != NULL)
        _value->~AbstractASTNode();
}

QString UnaryNode::writeIncrement(bool isPreincrement,bool isDecrement)
{
    QString oldVal = ir.writeCommandLine(QString("load %1, %1* %2%3")
                                         .arg(getValueTypeLLVM())
                                         .arg(_variable->isGlobal() ? "@" : "%")
                                         .arg(_variable->getUniqueName()));
    QString newVal;
    if(isDecrement){
        newVal = ir.writeCommandLine( QString("sub %1 %2, 1")
                                      .arg(getValueTypeLLVM())
                                      .arg(oldVal));
    }
    else {
        newVal = ir.writeCommandLine( QString("add %1 1, %2")
                                      .arg(getValueTypeLLVM())
                                      .arg(oldVal));
    }
    ir.writeLine(QString("store %1 %2, %1* %3")
                 .arg(getValueTypeLLVM())
                 .arg(newVal)
                 .arg((_variable->isGlobal() ? "@" : "%")+_variable->getUniqueName()));

    if(isPreincrement)
        return newVal;
    return oldVal;
}
