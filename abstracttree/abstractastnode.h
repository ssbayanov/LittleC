#ifndef ABSTRACTASTNODE_H
#define ABSTRACTASTNODE_H

#include <iostream>
#include <QString>
#include <QTextStream>

#include "symbolstable/abstractsymboltablerecord.h"
#include "irprint.h"

//#include "llvm/ADT/APFloat.h"
//#include "llvm/ADT/STLExtras.h"
//#include "llvm/IR/BasicBlock.h"
//#include "llvm/IR/Constants.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/Function.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/Module.h"
//#include "llvm/IR/Type.h"
//#include "llvm/IR/Verifier.h"

extern QTextStream treeStream;
extern QTextStream outStream;
extern IRPrint ir;

typedef enum
{
    NT_BinaryOperation,     // Binary operator.
    NT_UnaryOperation,      // Unary arithmetice operator.
    NT_AssignmentOperation, // Unary assignment operator.
    NT_NumericConstant,             // Numeric literal.
    NT_StringConstant,             // Sting literal.
    NT_Declaration,
    NT_Reference,        // Variable name.
    NT_IfStatement,       // If statement.
    NT_WhileStatement,    // While statement.
    NT_ForStatement,    // For statement
    NT_GoToStatement,     // Goto, Break or Continue statement.
    NT_LabelStatement,  // Label
    NT_SwitchStatement,     // Switch statement.
    NT_CaseStatement,   // Case statement
    NT_PintStatement,     // Print statement.
    NT_ScanExpression,
    NT_List,               // Expression or statement list.
    NT_FunctionDeclare,             // Declaration function.
    NT_FunctionCall,
    NT_FunctionReturn,
    NT_ArrayReference,
    NT_ArrayDeclare,
    NT_ArrayAssignment,
    NT_StructDeclare,
    NT_StructTypeDeclare,
    NT_StructReference
} ASTNodeTypeEnum;

class AbstractASTNode
{
public:
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
