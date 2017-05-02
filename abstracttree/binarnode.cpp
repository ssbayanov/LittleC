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
                  .arg(BinarNodeOperationText.at(_operation))
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

QString BinarNode::printTripleCode(int , QString )
{
    if(_left != NULL && _right != NULL) {
        if(_operation == BO_Assign) {
            ir.writeLine( QString("%1 = %2")
                         .arg(_left->printTripleCode())
                         .arg(_right->printTripleCode()));
        }
        else {
            QString operationLLVMText = "";
            ValueTypeEnum leftValueType = ((AbstractValueASTNode *)_left)->getValueType();

            switch(_operation) {
            case BO_Add:
                if(isInt(getValueType()))
                    operationLLVMText = "add";
                else
                    operationLLVMText = "fadd";
                break;
            case BO_Sub:
                if(isInt(getValueType()))
                    operationLLVMText = "sub";
                else
                    operationLLVMText = "fsub";
                break;
            case BO_Mul:
                if(isInt(getValueType()))
                    operationLLVMText = "mul";
                else
                    operationLLVMText = "fmul";
                break;
            case BO_Div:
                if(isInt(getValueType()))
                    operationLLVMText = "udiv";
                else
                    operationLLVMText = "fdiv";
                break;
            case BO_Rem:
                if(isInt(getValueType()))
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
                operationLLVMText = "err";
            }

            ir.writeCommandLine( QString("%2 %4 %1, %3")
                         .arg(
                              _left->printTripleCode(),
                              operationLLVMText,
                              _right->printTripleCode(),
                              getValueTypeLLVM())
                         );


            return QString(ir.lastCommandLine());
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

