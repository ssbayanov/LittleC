/*
* AST implementation
* Based on John Levine's book "flex&bison"
*/
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#include "myast.h"

NodeAST* CreateNodeAST(NodeTypeEnum nodetype, const char* opValue, NodeAST* left, NodeAST* right)
{
  NodeAST* a;
  try
  {
    a = new NodeAST;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  strcpy(a->opValue, opValue);
  a->valueType = left->valueType;
  a->left = left;
  a->right = right;
  return a;
}

NodeAST* CreateNumberNode(double doubleValue)
{
  TNumericValueNode* a;
  try
  {
    a = new TNumericValueNode;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }

  a->nodetype = typeConst;
  a->valueType = typeDouble;
  a->dNumber = doubleValue;

  return reinterpret_cast<NodeAST *>(a);
}

NodeAST* CreateNumberNode(int integerValue)
{
  TNumericValueNode* a;
  try
  {
    a = new TNumericValueNode;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }

  a->nodetype = typeConst;
  a->valueType = typeInt;
  a->iNumber = integerValue;
  return reinterpret_cast<NodeAST *>(a);
}

NodeAST* CreateTmpNode(int tmp_index)
{
  TTmpValueNode* a;
  try
  {
    a = new TTmpValueNode;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }

  a->nodetype = typeList;
  a->valueType = typeInt;
  a->Index = tmp_index;
  return reinterpret_cast<NodeAST *>(a);
}

NodeAST* CreateControlFlowNode(NodeTypeEnum nodetype, NodeAST* condition,
                              NodeAST* trueBranch, NodeAST* elseBranch
                              )
{
  TControlFlowNode* a;
  try
  {
    a = new TControlFlowNode;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }

  a->nodetype = nodetype;
  a->condition = condition;
  a->trueBranch = trueBranch;
  a->elseBranch = elseBranch;
  return reinterpret_cast<NodeAST *>(a);
}

NodeAST* CreateReferenceNode(TSymbolTableElementPtr symbol)
{
  TSymbolTableReference* a;
  try
  {
    a = new TSymbolTableReference;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }
  a->nodetype = typeIdentifier;
  a->variable = symbol;
  a->valueType = symbol->table->data[symbol->index].valueType;

  return reinterpret_cast<NodeAST *>(a);
}

NodeAST* CreateAssignmentNode(TSymbolTableElementPtr symbol, NodeAST* rightValue)
{
  TAssignmentNode* a;
  try
  {
    a = new TAssignmentNode;
  }
  catch (std::bad_alloc& ba)
  {
    perror("out of space");
    exit(0);
  }

  a->nodetype = typeAssignmentOp;
  a->variable = symbol;
  a->value = rightValue;

  return reinterpret_cast<NodeAST *>(a);
}

void FreeAST(NodeAST* a)
{
  if(NULL == a)
    return;
  switch(a->nodetype)
  {
  /* a pair of subtrees */
  case typeBinaryOp:
  case typeList:
    FreeAST(a->right);

    /* the only subtree */
  case typeUnaryOp:
    FreeAST(a->left);

    /* Terminal node */
  case typeConst:
  case typeIdentifier:
  case typeJumpStatement:
    break;
  case typeAssignmentOp:
    delete ((TAssignmentNode *)a)->value;
    break;
  case typeIfStatement:
  case typeWhileStatement:
    delete ((TControlFlowNode *)a)->condition;
    if( ((TControlFlowNode *)a)->trueBranch)
          FreeAST( ((TControlFlowNode *)a)->trueBranch);
    if( ((TControlFlowNode *)a)->elseBranch)
          FreeAST( ((TControlFlowNode *)a)->elseBranch);
    break;

  default: std::cout << "internal error: free bad node " << a->nodetype << std::endl;
  }

  delete a; /* Free the node itself */
}

/* AST dump */
void PrintAST(NodeAST* a, int level)
{
  std::cout << std::string (2 * level, ' '); /* indent to this level */
  ++level;

  if(NULL == a)
  {
    std::cout << "NULL" << std::endl;
    return;
  }

  switch(a->nodetype)
  {
    /* Numeric literal node */
  case typeConst:
    if(typeDouble == ((TNumericValueNode *)a)->valueType)
      std::cout << "dnumber " << ((TNumericValueNode *)a)->dNumber << std::endl;
    else if(typeInt == ((TNumericValueNode *)a)->valueType)
      std::cout << "inumber " << ((TNumericValueNode *)a)->iNumber << std::endl;
    else
      std::cout << "bad constant" << std::endl;
    break;

  case typeJumpStatement:
    std::cout << "goto" << std::endl;
    break;

  /* Symtable reference node */
  case typeIdentifier:
  {
    TSymbolTableElementPtr tmp = ((TSymbolTableReference *)a)->variable;
    std::cout << "ref ";
    if (NULL != tmp)
      std::cout << *(tmp->table->data[tmp->index].name);
    else
      std::cout << "(bad reference)";
    std::cout << std::endl;
  }
  break;

    /* Expression node */
  case typeList:
  case typeBinaryOp:
    std::cout << "binop " << a->opValue << std::endl;
    PrintAST(a->left, level);
    PrintAST(a->right, level);
    return;

  /* Unary operator node */
  case typeUnaryOp:
    std::cout << "unop " << a->opValue << std::endl;
    PrintAST(a->left, level);
    return;

    /* Assignment node */
  case typeAssignmentOp:
  {
    TSymbolTableElementPtr tmp = ((TAssignmentNode *)a)->variable;
    std::cout << "= ";
    if (NULL != tmp)
      std::cout << *(tmp->table->data[tmp->index].name);
    else
      std::cout << "(bad reference)";
    std::cout << std::endl;
    PrintAST( ((TAssignmentNode *)a)->value, level);
    return;
  }
  /* Control flow node - if */
  case typeIfStatement:
    std::cout << "flow - if" << std::endl;
    PrintAST( ((TControlFlowNode *)a)->condition, level);
    if( ((TControlFlowNode *)a)->trueBranch)
    {
      std::cout << std::string (2 * level, ' ');
      std::cout << "true-branch" << std::endl;
      PrintAST( ((TControlFlowNode *)a)->trueBranch, level + 1);
    }
    if( ((TControlFlowNode *)a)->elseBranch)
    {
      std::cout << std::string (2 * level, ' ');
      std::cout << "false-branch" << std::endl;
      PrintAST( ((TControlFlowNode *)a)->elseBranch, level + 1);
    }
    return;

  /* Control flow node - while */
  case typeWhileStatement:
    std::cout << "flow - while" << std::endl;
    PrintAST( ((TControlFlowNode *)a)->condition, level);
    if( ((TControlFlowNode *)a)->trueBranch)
    {
      std::cout << std::string (2 * level, ' ');
      std::cout << "loop-body" << std::endl;
      PrintAST( ((TControlFlowNode *)a)->trueBranch, level + 1);
    }
    return;

  default: std::cout << "bad node " << a->nodetype << std::endl;
    return;
  }
}
