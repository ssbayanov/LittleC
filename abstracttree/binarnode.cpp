#include "binarnode.h"

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operationText = operation;
    _operation = getOperation(operation);
    AbstractValueASTNode::_typeValue = ((AbstractValueASTNode *)left)->getValueType();
}

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operationText = operation;
    _operation = getOperation(operation);
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

QString BinarNode::printTripleCode(int level, QString param)
{
    if(_left != NULL && _right != NULL) {
        if(_operation == BO_Assign) {
            outStream << QString("%1 = %2")
                         .arg(_left->printTripleCode(level+1))
                         .arg(_right->printTripleCode(level+2));
        }
        else {
            QString operationLLVMText = "";
            ValueTypeEnum leftValueType = ((AbstractValueASTNode *)_left)->getValueType();
            bool isInt = (leftValueType == typeBool)
                    || (leftValueType == typeInt)
                    || (leftValueType == typeChar)
                    || (leftValueType == typeShort);

            switch(_operation) {
            case BO_Undef:
                operationLLVMText = "";
                break;
            case BO_Add:
                if(isInt)
                    operationLLVMText = "add";
                else
                    operationLLVMText = "fadd";
                break;
            case BO_Sub:
                if(isInt)
                    operationLLVMText = "sub";
                else
                    operationLLVMText = "fsub";
                break;
            case BO_Mul:
                if(isInt)
                    operationLLVMText = "mul";
                else
                    operationLLVMText = "fmul";
                break;
            case BO_Div:
                if(isInt)
                    operationLLVMText = "udiv";
                else
                    operationLLVMText = "fdiv";
                break;
            case BO_Rem:
                if(isInt)
                    operationLLVMText = "urem";
                else
                    operationLLVMText = "frem";
                break;
            case BO_And:
            case BO_AndBitwise:
            case BO_Or:
            case BO_OrBitwise:
            case BO_Xor:
            case BO_Lees:
            case BO_LeesEq:
            case BO_Eq:
            case BO_GreatEq:
            case BO_Great:
            case BO_NotEq:
            default:
                operationLLVMText = "";
            }

            outStream << QString("%t%1 = %3 %2, %4\n")
                         .arg(level)
                         .arg(_left->printTripleCode(level+1))
                         .arg(operationLLVMText)
                         .arg(_right->printTripleCode(level+2));
            return QString("%t%1")
                    .arg(level);
        }
    }
    return "";
}


BinarNode::~BinarNode()
{
    if (_left != NULL)
        _left->~AbstractASTNode();
    if (_right != NULL)
        _right->~AbstractASTNode();
}

BinarNodeOperationEnum BinarNode::getOperation(QString op)
{
    switch(op.toStdString().c_str()[0]) {
    case '+':
        return BO_Add;
        break;
    case '-':
        return BO_Sub;
        break;
    case '/':
        return BO_Div;
        break;
    case '*':
        return BO_Mul;
        break;
    case '%':
        return BO_Rem;
    case '!':
        return BO_NotEq;
        break;
    case '>':
        if(op.toStdString().length() > 1)
            return BO_GreatEq;
        return BO_Great;
        break;
    case '<':
        if(op.toStdString().length() > 1)
            return BO_LeesEq;
        return BO_Lees;
        break;
    case '&':
        if(op.toStdString().length() > 1)
            return BO_And;
        return BO_AndBitwise;
        break;
    case '|':
        if(op.toStdString().length() > 1)
            return BO_Or;
        return BO_OrBitwise;
        break;
    case '^':
        return BO_Xor;
        break;
    case '=':
        if(op.toStdString().length() > 1)
            return BO_Eq;
        return BO_Assign;
        break;
    default: return BO_Undef;
    }
}

