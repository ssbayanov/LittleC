#ifndef MYAST_H
#define MYAST_H
#include <string>
#include "symtable.h"
#include "subexpression.h"

typedef enum
{
    typeBinaryOp,     /* Binary operator. */
    typeUnaryOp,      /* Unary arithmetice operator. */
    typeAssignmentOp, /* Unary assignment operator. */
    typeConst,             /* Numeric literal. */
    typeStrConst,             /* Sting literal. */
    typeIdentifier,        /* Variable name. */
    typeIfStatement,       /* If statement. */
    typeWhileStatement,    /* While statement. */
    typeForStatement,    /* For statement */
    typeJumpStatement,     /* Goto, Break or Continue statement. */
    typeSwitchStatement,     /* Switch-Case statement. */
    typeInOutStatement,     /* Print-Scan statement. */
    typeList               /* Expression or statement list. */
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
NodeAST* CreateNodeAST(NodeTypeEnum cmptype, const char* opValue,
                                           NodeAST* left, NodeAST* right
                                          );
NodeAST* CreateNumberNode(double doubleValue);
NodeAST* CreateNumberNode(int integerValue);

NodeAST* CreateControlFlowNode(NodeTypeEnum Nodetype, NodeAST* condition,
                               NodeAST* trueBranch, NodeAST* elseBranch
                              );
NodeAST* CreateReferenceNode(TSymbolTableElementPtr symbol);
NodeAST* CreateAssignmentNode(TSymbolTableElementPtr symbol, NodeAST* rightValue);
NodeAST* CreateTmpNode(int tmp_index);

/* Freeing AST node from memory space */
void FreeAST(NodeAST *);

/* AST node dump */
void PrintAST(NodeAST* aTree, int level);

//typedef std::tuple<std::string, std::string, int> TokenTableRow;
extern std::vector <std::tuple<std::string, std::string, int> > TokenTable;

#endif // MYAST_H
