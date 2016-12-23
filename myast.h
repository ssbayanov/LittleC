#ifndef MYAST_H
#define MYAST_H
#include <string>
#include "symtable.h"
#include "subexpression.h"

typedef enum
{
    NT_BinaryOperation,     // Binary operator.
    NT_UnaryOperation,      // Unary arithmetice operator.
    NT_AssignmentOperation, // Unary assignment operator.
    NT_NumericConstant,             // Numeric literal.
    NT_StringConstant,             // Sting literal.
    NT_Identifier,        // Variable name.
    NT_IfStatement,       // If statement.
    NT_WhileStatement,    // While statement.
    NT_ForStatement,    // For statement
    NT_JumpStatement,     // Goto, Break or Continue statement.
    NT_SwitchStatement,     // Switch-Case statement.
    NT_InOutStatement,     // Print-Scan statement.
    NT_List               // Expression or statement list.
} NodeTypeEnum;


/* AST node declaration */
/* Each node has to have a given type attribute */

typedef struct TAbstractSyntaxTreeNode
{
  NodeTypeEnum nodetype;
  SubexpressionValueTypeEnum valueType;
  char opValue[256];
  struct TAbstractSyntaxTreeNode* left;
  struct TAbstractSyntaxTreeNode* right;
} NodeAST;

typedef struct
{
  NodeTypeEnum nodetype;        /* IfStatement or WhileStatement node */
  NodeAST* condition;  /* Condition expression */
  NodeAST* trueBranch; /* true branch statement */
  NodeAST* elseBranch; /* (optional) false branch statement */
} TControlFlowNode;

typedef struct
{
  NodeTypeEnum nodetype;			/* Type K */
  SubexpressionValueTypeEnum valueType;
  union
  {
    int    iNumber;
    short sNumber;
    double dNumber;
    float fNumber;
  };
} TNumericValueNode;

typedef struct
{
  NodeTypeEnum nodetype;
  SubexpressionValueTypeEnum valueType;
  union
  {
    char    cNumber;
  };
}TCharValueNode;

typedef struct
{
  NodeTypeEnum nodetype;
  SubexpressionValueTypeEnum valueType;
  TSymbolTableElementPtr variable;
} TSymbolTableReference;

typedef struct
{
  NodeTypeEnum nodetype;
  SubexpressionValueTypeEnum valueType;
  int Index;
} TTmpValueNode;

typedef struct
{
  NodeTypeEnum nodetype;
  TSymbolTableElementPtr variable;
  NodeAST* value;
} TAssignmentNode;

/* AST procedures declaration */
NodeAST* createNodeAST(NodeTypeEnum cmptype, const char* opValue,
                                           NodeAST* left, NodeAST* right
                                          );
NodeAST* createNumberNode(double doubleValue);
NodeAST* createNumberNode(int integerValue);

NodeAST* createControlFlowNode(NodeTypeEnum Nodetype, NodeAST* condition,
                               NodeAST* trueBranch, NodeAST* elseBranch
                              );
NodeAST* createReferenceNode(TSymbolTableElementPtr symbol);
NodeAST* createAssignmentNode(TSymbolTableElementPtr symbol, NodeAST* rightValue);
NodeAST* createTmpNode(int tmp_index);

/* Freeing AST node from memory space */
void freeAST(NodeAST *);

/* AST node dump */
void printAST(NodeAST* aTree, int level);

//typedef std::tuple<std::string, std::string, int> TokenTableRow;
extern std::vector <std::tuple<std::string, std::string, int> > TokenTable;

#endif // MYAST_H
