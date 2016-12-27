#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>

#include <QString>

#include "symbolstable.h"
#include "subexpression.h"

typedef enum
{
    NT_BinaryOperation,     // Binary operator.
    NT_UnaryOperation,      // Unary arithmetice operator.
    NT_AssignmentOperation, // Unary assignment operator.
    NT_NumericConstant,             // Numeric literal.
    NT_StringConstant,             // Sting literal.
    NT_Reference,        // Variable name.
    NT_IfStatement,       // If statement.
    NT_WhileStatement,    // While statement.
    NT_ForStatement,    // For statement
    NT_JumpStatement,     // Goto, Break or Continue statement.
    NT_SwitchStatement,     // Switch-Case statement.
    NT_InOutStatement,     // Print-Scan statement.
    NT_List               // Expression or statement list.
} ASTNodeTypeEnum;

typedef enum
{
    ToDouble,
    ToInt,
    ToBool,
    UnarMinus
}TypeUnary;

static QStringList typeString = QStringList() << "To double"
                                              << "To integer"
                                              << "To bool"
                                              << "Unar \"-\"";

class AbstractASTNode
{
public:
    AbstractASTNode(ASTNodeTypeEnum type);

    // Type
    void setType(ASTNodeTypeEnum type);
    ASTNodeTypeEnum getType();

    virtual void printNode(int level) = 0;

    virtual ~AbstractASTNode();

private:
    ASTNodeTypeEnum _nodetype;
};

class AbstractValueASTNode  : public AbstractASTNode
{
public:
    AbstractValueASTNode(ASTNodeTypeEnum type);

    virtual ValueTypeEnum getType();

    virtual ~AbstractValueASTNode();

    ValueTypeEnum _typeValue;
};

class ValueNode : public AbstractValueASTNode
{
public:
    ValueNode(ValueTypeEnum typeValue, QVariant value);

    void printNode(int level);

    ValueTypeEnum getType();

    ~ValueNode();

private:
    QVariant _value;

};

/**
 * @brief The ReferenceNode class Reference to variabel
 */
class ReferenceNode : public AbstractValueASTNode
{
public:
    ReferenceNode(SymbolsTableRecord *variable);

    void printNode(int level);

    ValueTypeEnum getType();

    ~ReferenceNode();

private:
    SymbolsTableRecord *_variable;
};

/**
 * @brief The ListNode class Set of expressions
 */
class ListNode : public AbstractASTNode
{
public:
    ListNode(AbstractASTNode *left, AbstractASTNode *right);

    void printNode(int level);

    ~ListNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
};

class IfNode : public AbstractASTNode
{
public:
    IfNode(AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch);

    void printNode(int level);

    ~IfNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_trueBranch;
    AbstractASTNode *_falseBranch;
};

/**
 * @brief The WhileNode class
 */
class WhileNode : public AbstractASTNode
{
public:
    WhileNode(AbstractASTNode *condition, AbstractASTNode *body);

    void printNode(int level);

    ~WhileNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_body;
};


/**
 * @brief The AssignmentNode class
 */
class AssignmentNode : public AbstractASTNode
{
public:
    AssignmentNode(SymbolsTableRecord *variable, AbstractASTNode *value);

    void printNode(int level);

    ~AssignmentNode();

private:
    SymbolsTableRecord *_variable;
    AbstractASTNode *_value;
};

/**
 * @brief The BinarNode class
 */
class BinarNode : public AbstractValueASTNode
{
public:
    BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation);

    void printNode(int level);

    ValueTypeEnum getType();

    ~BinarNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
    QString _operation;
};

class UnaryNode : public AbstractValueASTNode
{
public:
    UnaryNode(TypeUnary uType, AbstractASTNode *left);

    ValueTypeEnum getType();

    void printNode(int level);

    ~UnaryNode();

private:
    AbstractASTNode *_left;
    TypeUnary _uType;
};
#endif // ASTNODE_H
