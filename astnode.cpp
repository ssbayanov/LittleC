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
}

void WhileNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "While:"
              << std::endl;
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Condition:"
              << std::endl;
    _condition->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Body:"
              << std::endl;
    _body->printNode(level+1);
}

WhileNode::~WhileNode()
{
    _condition->~AbstractASTNode();
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
              << "Binar operation:"
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
}

ValueTypeEnum UnaryNode::getType()
{
    return typeDouble;
}

void UnaryNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Unar operation. Type: %1")
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
