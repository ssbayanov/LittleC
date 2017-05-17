#ifndef ABSTRACTASTNODE_H
#define ABSTRACTASTNODE_H

#include <iostream>
#include <QString>
#include <QTextStream>

#include "symbolstable/abstractsymboltablerecord.h"
#include "irprint.h"

extern QTextStream treeStream;
extern IRPrint ir;

typedef enum
{
    NT_BinaryOperation,     // Binary operator.
    NT_UnaryOperation,      // Unary arithmetice operator.
    NT_AssignmentOperation, // Unary assignment operator.
    NT_NumericConstant,     // Numeric literal.
    NT_StringConstant,      // Sting literal.
    NT_Declaration,         // Variable declaration
    NT_Reference,           // Variable name.
    NT_IfStatement,         // If statement.
    NT_WhileStatement,      // While statement.
    NT_ForStatement,        // For statement
    NT_GoToStatement,       // Goto, Break or Continue statement.
    NT_LabelStatement,      // Label
    NT_SwitchStatement,     // Switch statement.
    NT_CaseStatement,       // Case statement
    NT_PintStatement,       // Print statement.
    NT_ScanExpression,      // Scan function
    NT_List,                // List of nodes.
    NT_FunctionDeclare,     // Declaration function.
    NT_FunctionCall,        // Calling function
    NT_FunctionReturn,
    NT_ArrayReference,
    NT_ArrayDeclare,
    NT_ArrayAssignment,
    NT_StructDeclare,
    NT_StructTypeDeclare,
    NT_StructReference,
    NT_StructAssignment
} ASTNodeTypeEnum;

/**
 * @brief The AbstractASTNode class of node abstract tree
 */

class AbstractASTNode
{
public:
    /**
     * @brief AbstractASTNode
     * @param type
     */
    AbstractASTNode(ASTNodeTypeEnum type);

    // Type
    void setType(ASTNodeTypeEnum type);
    ASTNodeTypeEnum getType();

    virtual void printNode(int level = 0);

    virtual QString printTripleCode();

    virtual ~AbstractASTNode();

    virtual bool isValueNode();

private:
    ASTNodeTypeEnum _nodetype;
};

#endif // ABSTRACTASTNODE_H
