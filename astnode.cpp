#include "astnode.h"

AbstractASTNode::AbstractASTNode(ASTNodeTypeEnum type)
{
    _nodetype = type;
}

void AbstractASTNode::setType(ASTNodeTypeEnum type)
{
    _nodetype = type;
}

ASTNodeTypeEnum AbstractASTNode::getType()
{
    return _nodetype;
}

void AbstractASTNode::printNode(int level)
{
    std::cout << QString("printing Abstract node :( on level %1").arg(level).toStdString();
}

AbstractASTNode::~AbstractASTNode()
{

}


//----------------------------------------------------------------------------------------------------------------------
//ValueNode
//----------------------------------------------------------------------------------------------------------------------
AbstractValueASTNode::AbstractValueASTNode(ASTNodeTypeEnum type)
    : AbstractASTNode(type)
{

}

ValueTypeEnum AbstractValueASTNode::getType()
{
    return _typeValue;
}

AbstractValueASTNode::~AbstractValueASTNode()
{

}


//----------------------------------------------------------------------------------------------------------------------
//ValueNode
//----------------------------------------------------------------------------------------------------------------------
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

ValueTypeEnum ValueNode::getType()
{
    return _typeValue;
}

ValueNode::~ValueNode()
{

}

//----------------------------------------------------------------------------------------------------------------------
//ReferenceNode
//----------------------------------------------------------------------------------------------------------------------
ReferenceNode::ReferenceNode(SymbolsTableRecord *variable)
    : AbstractValueASTNode(NT_Reference)
{
    _variable = variable;
    _typeValue = _variable->valueType;
}

void ReferenceNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Reference: %1, type: %2").arg(_variable->name).arg(typeName.at(_typeValue)).toStdString()
              << std::endl;
}

ValueTypeEnum ReferenceNode::getType()
{
    return _variable->valueType;
}

ReferenceNode::~ReferenceNode()
{

}

//----------------------------------------------------------------------------------------------------------------------
//FunctionDeclareNode
//----------------------------------------------------------------------------------------------------------------------
FunctionDeclareNode::FunctionDeclareNode(SymbolsTableRecord *variable, AbstractASTNode *paramList, AbstractASTNode *body)
    : AbstractValueASTNode(NT_FunctionDeclare)
{
    _variable = variable;
    _paramList = paramList;
    _body = body;
}

void FunctionDeclareNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Declaration function: %1, type: %2")
                 .arg(_variable->name)
                 .arg(typeName.at(_variable->valueType)).toStdString()
              << std::endl;

    if (_paramList != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Params:"
                  << std::endl;
        _paramList->printNode(level+1);
    }

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Body:"
              << std::endl;
    _body->printNode(level+1);
}

FunctionDeclareNode::~FunctionDeclareNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
    _body->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//FunctionCallNode
//----------------------------------------------------------------------------------------------------------------------
FunctionCallNode::FunctionCallNode(SymbolsTableRecord *variable, AbstractASTNode *paramList)
    : AbstractValueASTNode(NT_FunctionCall)
{
    _variable = variable;
    _paramList = paramList;
    _typeValue = _variable->valueType;
}

void FunctionCallNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Call function: %1, type: %2")
                 .arg(_variable->name)
                 .arg(typeName.at(_variable->valueType)).toStdString()
              << std::endl;

    if (_paramList != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Params:"
                  << std::endl;
        _paramList->printNode(level+1);
    }
}

FunctionCallNode::~FunctionCallNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//FunctionCallNode
//----------------------------------------------------------------------------------------------------------------------
FunctionReturnNode::FunctionReturnNode(AbstractASTNode *expression)
    : AbstractValueASTNode(NT_FunctionReturn)
{
    _expression = expression;
    _typeValue = ((AbstractValueASTNode *)expression)->getType();
}

void FunctionReturnNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Return type %1")
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Expression:"
              << std::endl;
    _expression->printNode(level+1);

}

FunctionReturnNode::~FunctionReturnNode()
{
    _expression->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//BinarNode
//----------------------------------------------------------------------------------------------------------------------
BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
    _typeValue = ((AbstractValueASTNode *)left)->getType();
}

BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
    _typeValue = typeValue;
}

void BinarNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Binar operation: %1, type: %2")
                 .arg(_operation)
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Left:"
              << std::endl;
    _left->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Right:"
              << std::endl;
    _right->printNode(level+1);
}

ValueTypeEnum BinarNode::getType()
{
    return typeDouble;
}

BinarNode::~BinarNode()
{
    _left->~AbstractASTNode();
    _right->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//UnaryNode
//----------------------------------------------------------------------------------------------------------------------
UnaryNode::UnaryNode(TypeUnary uType, AbstractASTNode *left)
    : AbstractValueASTNode(NT_UnaryOperation)
{
    _uType = uType;
    _left = left;
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
        _typeValue = ((AbstractValueASTNode *)_left)->getType();
    }
}

ValueTypeEnum UnaryNode::getType()
{
    return _typeValue;
}

void UnaryNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Unar operation. Type: %1. Type of result: %2")
                 .arg(unarTypeString.at(_uType))
                 .arg(typeName.at(_typeValue)).toStdString()
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Left:"
              << std::endl;
    _left->printNode(level+1);

}

UnaryNode::~UnaryNode()
{
    _left->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//ListNode
//----------------------------------------------------------------------------------------------------------------------
ListNode::ListNode(AbstractASTNode *left, AbstractASTNode *right)
    : AbstractASTNode(NT_List)
{
    _left = left;
    _right = right;
}

void ListNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "List:"
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Left:"
              << std::endl;

    if(_left != NULL)
        _left->printNode(level+1);
    else
        std::cout << "BAD LEFT LIST NODE";

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Right:"
              << std::endl;

    if(_right != NULL)
        _right->printNode(level+1);
    else
        std::cout << "BAD RIGHT LIST NODE";

}

ListNode::~ListNode()
{
    _left->~AbstractASTNode();
    _right->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//IfNode
//----------------------------------------------------------------------------------------------------------------------
IfNode::IfNode(AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch)
    : AbstractASTNode(NT_IfStatement)
{
    _condition = condition;
    _trueBranch = trueBranch;
    _falseBranch = falseBranch;
}

void IfNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "If"
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Condition:"
              << std::endl;
    _condition->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "True branch:"
              << std::endl;
    _trueBranch->printNode(level+1);
    if (_falseBranch != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "False branch:"
                  << std::endl;
        _falseBranch->printNode(level+1);
    }
}

IfNode::~IfNode()
{
    _condition->~AbstractASTNode();
    _trueBranch->~AbstractASTNode();
    if (_falseBranch != NULL)
        _falseBranch->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//WhileNode
//----------------------------------------------------------------------------------------------------------------------
WhileNode::WhileNode(AbstractASTNode *condition, AbstractASTNode *body)
    : AbstractASTNode(NT_WhileStatement)
{
    _condition = condition;
    _body = body;
    _isDoWhile = false;
}

void WhileNode::setBody(AbstractASTNode *body)
{
    _body = body;
}

void WhileNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << (_isDoWhile == false ? "While:" : "Do-While")
              << std::endl;


    if(!_isDoWhile && _condition != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Condition:"
                  << std::endl;
        _condition->printNode(level+1);
    }

    if(_body != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Body:"
                  << std::endl;
        _body->printNode(level+1);
    }
    if(_isDoWhile && _condition != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Condition:"
                  << std::endl;
        _condition->printNode(level+1);
    }
}

void WhileNode::setIsDoWhile(bool isDoWhile)
{
    _isDoWhile = isDoWhile;
}

WhileNode::~WhileNode()
{
    if(_condition != NULL)
        _condition->~AbstractASTNode();
    _body->~AbstractASTNode();
}


//----------------------------------------------------------------------------------------------------------------------
//ForNode
//----------------------------------------------------------------------------------------------------------------------
ForNode::ForNode(AbstractASTNode *init, AbstractASTNode *condition, AbstractASTNode *increment, AbstractASTNode *body)
    : AbstractASTNode(NT_ForStatement)
{
    _init = init;
    _condition = condition;
    _increment = increment;
    _body = body;
}

void ForNode::setBody(AbstractASTNode *body)
{
    _body = body;
}

void ForNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "For:"
              << std::endl;
    if(_init != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Initialisation:"
                  << std::endl;
        _init->printNode(level+1);
    }

    if(_condition != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Condition:"
                  << std::endl;
        _condition->printNode(level+1);
    }

    if(_increment != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Increment:"
                  << std::endl;
        _increment->printNode(level+1);
    }

    if(_body != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Body:"
                  << std::endl;
        _body->printNode(level+1);
    }
}

ForNode::~ForNode()
{
    if(_init != NULL)
        _init->~AbstractASTNode();
    if(_condition != NULL)
        _condition->~AbstractASTNode();
    if(_increment != NULL)
        _increment->~AbstractASTNode();
    _body->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//AssignmentNode
//----------------------------------------------------------------------------------------------------------------------
AssignmentNode::AssignmentNode(SymbolsTableRecord *variable, AbstractASTNode *value)
    : AbstractASTNode(NT_AssignmentOperation)
{
    _variable = variable;
    _value = value;
}

void AssignmentNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Assignment: %1").arg(_variable->name).toStdString()
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Value:"
              << std::endl;
    _value->printNode(level+1);
}

AssignmentNode::~AssignmentNode()
{
    _value->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//LabelNode
//----------------------------------------------------------------------------------------------------------------------
LabelNode::LabelNode(SymbolsTableRecord *variable)
    : AbstractASTNode(NT_LabelStatement)
{
    _variable = variable;
}

void LabelNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Label: %1").arg(_variable->name).toStdString()
              << std::endl;
}

LabelNode::~LabelNode()
{

}

//----------------------------------------------------------------------------------------------------------------------
//GoToNode
//----------------------------------------------------------------------------------------------------------------------
GoToNode::GoToNode(QString nameLabel)
    : AbstractASTNode(NT_GoToStatement)
{
    _nameLabel = nameLabel;
}

void GoToNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Go to label: %1").arg(_nameLabel).toStdString()
              << std::endl;
}

GoToNode::~GoToNode()
{

}

//----------------------------------------------------------------------------------------------------------------------
//CaseNode
//----------------------------------------------------------------------------------------------------------------------
CaseNode::CaseNode(AbstractASTNode *value, AbstractASTNode *expressions)
    : AbstractASTNode(NT_CaseStatement)
{
    _value = value;
    _expressions = expressions;
}

void CaseNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("%1:").arg(_value != NULL ? "case" : "default").toStdString()
              << std::endl;

    if(_value != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Value:"
                  << std::endl;
        _value->printNode(level+1);
    }
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Expressions:"
              << std::endl;
    _expressions->printNode(level+1);
}

CaseNode::~CaseNode()
{
    _expressions->~AbstractASTNode();
    _value->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//SwitchNode
//----------------------------------------------------------------------------------------------------------------------
SwitchNode::SwitchNode(AbstractASTNode *value, AbstractASTNode *caseList)
    : AbstractASTNode(NT_SwitchStatement)
{
    _value = value;
    _caseList = caseList;
}

void SwitchNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Switch:"
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Value:"
              << std::endl;
    _value->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Case list:"
              << std::endl;
    _caseList->printNode(level+1);
}

SwitchNode::~SwitchNode()
{
    _caseList->~AbstractASTNode();
    _value->~AbstractASTNode();
}

//----------------------------------------------------------------------------------------------------------------------
//PrintNode
//----------------------------------------------------------------------------------------------------------------------
PrintNode::PrintNode(AbstractASTNode *expression)
    : AbstractASTNode(NT_PintStatement)
{
    _expression = expression;
}

void PrintNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Print:"
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Expression:"
              << std::endl;
    _expression->printNode(level+1);
}

PrintNode::~PrintNode()
{
    _expression->~AbstractASTNode();
}

