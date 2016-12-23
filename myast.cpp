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

NodeAST* createNodeAST(NodeTypeEnum nodetype, const char* opValue, NodeAST* left, NodeAST* right)
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

NodeAST* createNumberNode(double doubleValue)
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

    a->nodetype = NT_NumericConstant;
    a->valueType = typeDouble;
    a->dNumber = doubleValue;

    return reinterpret_cast<NodeAST *>(a);
}

NodeAST* createNumberNode(int integerValue)
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

    a->nodetype = NT_NumericConstant;
    a->valueType = typeInt;
    a->iNumber = integerValue;
    return reinterpret_cast<NodeAST *>(a);
}

NodeAST* createTmpNode(int tmp_index)
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

    a->nodetype = NT_List;
    a->valueType = typeInt;
    a->Index = tmp_index;
    return reinterpret_cast<NodeAST *>(a);
}

NodeAST* createControlFlowNode(NodeTypeEnum nodetype, NodeAST* condition,
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

NodeAST* createReferenceNode(TSymbolTableElementPtr symbol)
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
    a->nodetype = NT_Identifier;
    a->variable = symbol;
    a->valueType = symbol->table->data[symbol->index].valueType;

    return reinterpret_cast<NodeAST *>(a);
}

NodeAST* createAssignmentNode(TSymbolTableElementPtr symbol, NodeAST* rightValue)
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

    a->nodetype = NT_AssignmentOperation;
    a->variable = symbol;
    a->value = rightValue;

    return reinterpret_cast<NodeAST *>(a);
}

void freeAST(NodeAST* a)
{
    if(NULL == a)
        return;
    switch(a->nodetype)
    {
    /* a pair of subtrees */
    case NT_BinaryOperation:
    case NT_List:
        freeAST(a->right);

        /* the only subtree */
    case NT_UnaryOperation:
        freeAST(a->left);

        /* Terminal node */
    case NT_NumericConstant:
    case NT_Identifier:
    case NT_JumpStatement:
        break;
    case NT_AssignmentOperation:
        delete ((TAssignmentNode *)a)->value;
        break;
    case NT_IfStatement:
    case NT_WhileStatement:
        delete ((TControlFlowNode *)a)->condition;
        if( ((TControlFlowNode *)a)->trueBranch)
            freeAST( ((TControlFlowNode *)a)->trueBranch);
        if( ((TControlFlowNode *)a)->elseBranch)
            freeAST( ((TControlFlowNode *)a)->elseBranch);
        break;

    default: std::cout << "internal error: free bad node " << a->nodetype << std::endl;
    }

    delete a; /* Free the node itself */
}

/* AST dump */
void printAST(NodeAST* a, int level)
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
    case NT_NumericConstant:
        if(typeDouble == ((TNumericValueNode *)a)->valueType)
            std::cout << "double number " << ((TNumericValueNode *)a)->dNumber << std::endl;
        else if(typeInt == ((TNumericValueNode *)a)->valueType)
            std::cout << "integer number " << ((TNumericValueNode *)a)->iNumber << std::endl;
        else
            std::cout << "bad constant" << std::endl;
        break;

    case NT_JumpStatement:
        std::cout << "goto" << std::endl;
        break;

        /* Symtable reference node */
    case NT_Identifier:
    {
        TSymbolTableElementPtr tmp = ((TSymbolTableReference *)a)->variable;
        std::cout << "Reference: ";
        if (NULL != tmp)
            std::cout << *(tmp->table->data[tmp->index].name);
        else
            std::cout << "(Bad reference)";
        std::cout << std::endl;
    }
        break;

        /* Expression node */
    case NT_List:
    case NT_BinaryOperation:
        std::cout << "Binar operation: " << a->opValue << std::endl;
        printAST(a->left, level);
        printAST(a->right, level);
        return;

        /* Unary operator node */
    case NT_UnaryOperation:
        std::cout << "Unar operation " << a->opValue << std::endl;
        printAST(a->left, level);
        return;

        /* Assignment node */
    case NT_AssignmentOperation:
    {
        TSymbolTableElementPtr tmp = ((TAssignmentNode *)a)->variable;
        std::cout << "= ";
        if (NULL != tmp)
            std::cout << *(tmp->table->data[tmp->index].name);
        else
            std::cout << "(Bad reference)";
        std::cout << std::endl;
        printAST( ((TAssignmentNode *)a)->value, level);
        return;
    }
        /* Control flow node - if */
    case NT_IfStatement:
        std::cout << "Flow - if: " << std::endl;
        printAST( ((TControlFlowNode *)a)->condition, level);
        if( ((TControlFlowNode *)a)->trueBranch)
        {
            std::cout << std::string (2 * level, ' ');
            std::cout << "True-branch: " << std::endl;
            printAST( ((TControlFlowNode *)a)->trueBranch, level + 1);
        }
        if( ((TControlFlowNode *)a)->elseBranch)
        {
            std::cout << std::string (2 * level, ' ');
            std::cout << "False-branch: " << std::endl;
            printAST( ((TControlFlowNode *)a)->elseBranch, level + 1);
        }
        return;

        /* Control flow node - while */
    case NT_WhileStatement:
        std::cout << "Flow - while: " << std::endl;
        std::cout << std::string (2 * level, ' ');
        std::cout << "While condition: " << std::endl;
        printAST( ((TControlFlowNode *)a)->condition, level + 1);
        if( ((TControlFlowNode *)a)->trueBranch)
        {
            std::cout << std::string (2 * level, ' ');
            std::cout << "Loop-body: " << std::endl;
            printAST( ((TControlFlowNode *)a)->trueBranch, level + 1);
        }
        return;

    default: std::cout << "Bad node: " << a->nodetype << std::endl;
        return;
    }
}
