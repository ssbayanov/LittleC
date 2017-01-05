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
    NT_GoToStatement,     // Goto, Break or Continue statement.
    NT_LabelStatement,  // Label
    NT_SwitchStatement,     // Switch statement.
    NT_CaseStatement,   // Case statement
    NT_PintStatement,     // Print statement.
    NT_List               // Expression or statement list.
} ASTNodeTypeEnum;

typedef enum
{
    UnarToDouble,
    UnarToInt,
    UnarToBool,
    UnarMinus,
    UnarNot
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
    WhileNode(AbstractASTNode *condition, AbstractASTNode *body = NULL);

    void setBody(AbstractASTNode *body);

    void printNode(int level);

    void setIsDoWhile(bool isDoWhile = true);

    ~WhileNode();

private:
    AbstractASTNode *_condition;
    AbstractASTNode *_body;
    bool _isDoWhile;
};

/**
 * @brief The ForNode class
 */
class ForNode : public AbstractASTNode
{
public:
    ForNode(AbstractASTNode *init, AbstractASTNode *condition, AbstractASTNode *increment, AbstractASTNode *body = NULL);

    void setBody(AbstractASTNode *body);

    void printNode(int level);

    ~ForNode();

private:
    AbstractASTNode *_init;
    AbstractASTNode *_condition;
    AbstractASTNode *_increment;
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

    BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue);

    void printNode(int level);

    ValueTypeEnum getType();

    ~BinarNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
    QString _operation;
};

/**
 * @brief The UnaryNode class node for storage unary operations
 */
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
    ValueTypeEnum _typeValue;
};

/**
 * @brief The LabelNode class
 */
class LabelNode : public AbstractASTNode
{
public:
    LabelNode(SymbolsTableRecord *variable);

    void printNode(int level);

    ~LabelNode();

private:
    SymbolsTableRecord *_variable;
};

/**
 * @brief The GoToNode class
 */
class GoToNode : public AbstractASTNode
{
public:
    GoToNode(QString nameLabel);

    void printNode(int level);

    ~GoToNode();

private:
    QString _nameLabel;
};

/**
 * @brief The CaseNode class
 */
class CaseNode : public AbstractASTNode
{
public:
    CaseNode(AbstractASTNode *value, AbstractASTNode *expressions);

    void printNode(int level);

    ~CaseNode();

private:
    AbstractASTNode *_value;
    AbstractASTNode *_expressions;
};

/**
 * @brief The SwitchNode class
 */
class SwitchNode : public AbstractASTNode
{
public:
    /**
     * @brief SwitchNode construcotr of class
     * @param value pointer to value node
     * @param caseList pointer to cases list node
     */
    SwitchNode(AbstractASTNode *value, AbstractASTNode *caseList);

    /**
     * @brief printNode printing switch node to stdout
     * @param level
     */
    void printNode(int level);

    /**
      * @brief ~SwitchNode destruction class and call destrucor of _value and _caseList
      */
    ~SwitchNode();

private:
    /**
     * @brief _value pointer to switching value or expression
     */
    AbstractASTNode *_value;

    /**
     * @brief _caseList pointer to cases list
     */
    AbstractASTNode *_caseList;

};

/**
 * @brief The PrintNode class
 */
class PrintNode : public AbstractASTNode
{
public:
    PrintNode(AbstractASTNode *expression);

    void printNode(int level);

    ~PrintNode();

private:
    AbstractASTNode *_expression;
};

#endif // ASTNODE_H

