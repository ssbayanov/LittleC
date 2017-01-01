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
    std::cout << "printing Abstract node :(";
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
              << QString("Value: %1").arg(_value.toString()).toStdString()
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
}

void ReferenceNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Reference: %1").arg(_variable->name).toStdString()
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
//BinarNode
//----------------------------------------------------------------------------------------------------------------------
BinarNode::BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation)
    : AbstractValueASTNode(NT_BinaryOperation)
{
    _left = left;
    _right = right;
    _operation = operation;
}

void BinarNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Binar operation: %1").arg(_operation).toStdString()
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
    case ToInt:
        _typeValue = typeInt;
        break;
    case ToDouble:
        _typeValue = typeDouble;
        break;
    case ToBool:
        _typeValue = typeBool;
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
              << QString("Unar operation. Type of result: %1")
                 .arg(typeName.at(_uType)).toStdString()
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
    _left->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Right:"
              << std::endl;
    _right->printNode(level+1);
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

    std::cout << QString().fill(' ',level*2).toStdString()
              << "False branch:"
              << std::endl;
    _falseBranch->printNode(level+1);
}

IfNode::~IfNode()
{
    _condition->~AbstractASTNode();
    _trueBranch->~AbstractASTNode();
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

    if(!_isDoWhile) {
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
    if(_isDoWhile) {
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

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Body:"
              << std::endl;
    _body->printNode(level+1);
}

ForNode::~ForNode()
{
    _init->~AbstractASTNode();
    _condition->~AbstractASTNode();
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


