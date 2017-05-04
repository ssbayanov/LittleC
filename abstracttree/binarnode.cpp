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
                if(isInt(leftValueType))
                    operationLLVMText = "add";
                else
                    operationLLVMText = "fadd";
                break;
            case BO_Sub:
                if(isInt(leftValueType))
                    operationLLVMText = "sub";
                else
                    operationLLVMText = "fsub";
                break;
            case BO_Mul:
                if(isInt(leftValueType))
                    operationLLVMText = "mul";
                else
                    operationLLVMText = "fmul";
                break;
            case BO_Div:
                if(isInt(leftValueType))
                    operationLLVMText = "udiv";
                else
                    operationLLVMText = "fdiv";
                break;
            case BO_Rem:
                if(isInt(leftValueType))
                    operationLLVMText = "urem";
                else
                    operationLLVMText = "frem";
                break;
            case BO_And:
            case BO_AndBitwise:
                operationLLVMText = "and";
                break;
            case BO_Or:
            case BO_OrBitwise:
                operationLLVMText = "or";
                break;
            case BO_Xor:
                operationLLVMText = "xor";
                break;
            case BO_Lees:
                if(isInt(leftValueType))
                    operationLLVMText = "icmp slt";
                else
                    operationLLVMText = "fcmp olt";
                break;
            case BO_LeesEq:
                if(isInt(leftValueType))
                    operationLLVMText = "icmp sle";
                else
                    operationLLVMText = "fcmp ole";
                break;
            case BO_Eq:
                if(isInt(leftValueType))
                    operationLLVMText = "icmp eq";
                else
                    operationLLVMText = "fcmp oeq";
                break;
            case BO_GreatEq:
                if(isInt(leftValueType))
                    operationLLVMText = "icmp sge";
                else
                    operationLLVMText = "fcmp oge";
                break;
            case BO_Great:
                if(isInt(leftValueType))
                    operationLLVMText = "icmp sgt";
                else
                    operationLLVMText = "fcmp ogt";
                break;
            case BO_NotEq:
                if(isInt(leftValueType))
                    operationLLVMText = "icmp ne";
                else
                    operationLLVMText = "fcmp one";
                break;
            default:
                operationLLVMText = "err";
            }

            ir.writeCommandLine( QString("%1 %2 %3, %4")
                                 .arg(
                                     operationLLVMText,
                                     getValueTypeLLVM(leftValueType),
                                     _left->printTripleCode(),
                                     _right->printTripleCode()
                                     )
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

