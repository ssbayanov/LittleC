#ifndef ABSTRACTASTNODE_H
#define ABSTRACTASTNODE_H

#include <iostream>
#include <QString>
#include <QTextStream>

#include "symbolstable/abstractsymboltablerecord.h"
#include "irprint.h"

extern QTextStream treeStream;
extern IRPrint ir;

/// Enum type. Type of abstract tree node
typedef enum
{
    NT_BinaryOperation,     ///< Binary operator.
    NT_UnaryOperation,      ///< Unary arithmetice operator.
    NT_AssignmentOperation, ///< Unary assignment operator.
    NT_NumericConstant,     ///< Numeric literal.
    NT_StringConstant,      ///< Sting literal.
    NT_Declaration,         ///< Variable declaration
    NT_Reference,           ///< Variable name.
    NT_IfStatement,         ///< If statement.
    NT_WhileStatement,      ///< While statement.
    NT_ForStatement,        ///< For statement
    NT_GoToStatement,       ///< Goto, Break or Continue statement.
    NT_LabelStatement,      ///< Label
    NT_SwitchStatement,     ///< Switch statement.
    NT_CaseStatement,       ///< Case statement
    NT_PintStatement,       ///< Print statement.
    NT_ScanExpression,      ///< Scan function
    NT_List,                ///< List of nodes.
    NT_FunctionDeclare,     ///< Declaration function.
    NT_FunctionCall,        ///< Calling function
    NT_FunctionReturn,      ///< Return node
    NT_ArrayReference,      ///< Reference of array element
    NT_ArrayDeclare,        ///< Array declaration
    NT_ArrayAssignment,     ///< Array element assignment
    NT_StructDeclare,       ///< Declaration variable of struct type
    NT_StructTypeDeclare,   ///< Declaration struct
    NT_StructReference,     ///< Struct variable refference
    NT_StructAssignment     ///< Struct varaible assignment
} ASTNodeTypeEnum;

/**
 * @brief The AbstractASTNode class of node abstract tree.
 */
class AbstractASTNode
{
public:
    /**
     * @brief AbstractASTNode Construcotor of class.
     * @param type Type of node.
     */
    AbstractASTNode(ASTNodeTypeEnum type);

    /**
     * @brief setType Set type of node.
     * @param type New type of node.
     */
    void setType(ASTNodeTypeEnum type);

    /**
     * @brief getType Get type of node.
     * @return Type of node.
     */
    ASTNodeTypeEnum getType();

    /**
     * @brief printNode Virtual function of printing tree. Tree printed to treeStream.
     * @param level Level of node. Setup of space.
     */
    virtual void printNode(int level = 0);

    /**
     * @brief printTripleCode Virtual function of printing tree.
     * Tree printed to outStream IR LLVM code.
     * @return return variable in IR LLVM if node hase value.
     */
    virtual QString printTripleCode();

    /**
     * @brief isValueNode Verification is AbstractrVlueNode.
     * @return If is AbstractrVlueNode return true, otherwise return false.
     */
    virtual bool isValueNode();

    /**
     * @brief ~AbstractASTNode destructor of class
     */
    virtual ~AbstractASTNode();

private:
    /**
     * @brief _nodetype Variable contain of an abstract node type
     */
    ASTNodeTypeEnum _nodetype;
};

#endif // ABSTRACTASTNODE_H
