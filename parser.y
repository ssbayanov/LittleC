%code requires { //code insert in top parser_yacc.h
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <QString>
#include <QStringList>
#include "symbolstable.h"
#include "myast.h"
}

%code provides {
    void parsererror(QString s);

    // Errors
    enum {  ERROR_DOUBLE_DECLARED,
            ERROR_NOT_DECLARED,
            WARNING_TYPES_INCOMPATIBLE,
            ERROR_BREAK_NOT_INSIDE_LOOP,
            ERROR_MEMORY_ALLOCATION,
            ERROR_UNRECOGNIZED_TOKEN,
            ERROR_UNFINISHED_STRING,
            ERROR_UNCLOSED_COMMENT};

    static QStringList errorList = QStringList() << "error: Variable %1 is already declared."
                              << "error: Variable %1 was not declared."
                              << "warning: types in %1 opertion incompatible."
                              << "error: 'break' not inside loop."
                              << "error: memory allocation or access error."
                              << "error: %1 - unrecognized token."
                              << "error: unfinished string."
                              << "error: nonclosed comment";

}

%{
#include "parser_yacc.h"

/* Обработка синтаксического дерева */
#define ADDITION_OPERATOR       1
#define SUBTRACTION_OPERATOR    2
#define MULTIPLICATION_OPERATOR 3
#define DIVISION_OPERATOR       4
#define NEGATION_OPERATOR       5
#define UNPLUS_OPERATOR         6
#define IF_FALSE_GOTO_OPERATOR 	7
#define IF_TRUE_GOTO_OPERATOR 	8
#define GOTO_OPERATOR           9
#define SET_LABEL_OPERATOR      10
#define OUTPUT_OPERATOR         11
#define ASSIGN_OPERATOR         12
#define UNMINUS_OPERATOR        13
#define MOD_OPERATOR            14
#define AND_OPERATOR            15
#define OR_OPERATOR             16
#define XOR_OPERATOR            17
#define NOT_OPERATOR            18
#define MORE_OPERATOR           19
#define LESS_OPERATOR           20
#define MEQ_OPERATOR            21
#define LEQ_OPERATOR            22
#define EQ_OPERATOR             23
#define NEQ_OPERATOR            24
#define BRACKET_OPERATOR        25
#define PAREN_OPERATOR          26
#define BRACE_OPERATOR          27
#define COLON_OPERATOR          28
#define DOT_OPERATOR            29
#define COMA_OPERATOR           30
#define PRINT_OPERATOR          31
#define SCAN_OPERATOR           32

#define YYERROR_VERBOSE         1


/*#define YYDEBUG 1
#define yyoutput std::cout*/

/* описание структуры синтаксического дерева */
int g_tmpvar_upper_index = 0;

/*NodeAST* constants(double value);
NodeAST* idents (int index);
NodeAST* tmpvars (int tmp_index);*/
/* Генерация трехадресного кода */
/*int codegenBinary(FILE* outputFile, int operatorCode, NodeAST* leftOperand, NodeAST* rightOperand, NodeAST* result);
int codegenUnary(FILE* outputFile, int operatorCode, NodeAST* operand, NodeAST* result);
int codegenGoto(FILE* outputFile, int operatorCode,int labelREALCONST, NodeAST* optionalExpression);
int codegenLabel(FILE* outputFile, int labelREALCONST);*/

/* Обработка таблицы меток. Используется стековая организация */
static int g_LastLabelREALCONST = 0;
static int g_LabelStackPointer = 0;
static int Labels[256];
static void PushLabelREALCONST(int);
static int PopLabelREALCONST(void);
static void EmptyLabels(void);

extern int lno;
extern int parserlex();
extern char g_outFileName[256]; /* Имя выходного файла */

//#undef yyerror
//#define yyerror driver.error

int g_LoopNestingCounter = 0;

static SymbolsTable* topLevelVariableTable = new SymbolsTable(NULL);
static SymbolsTable* currentTable = topLevelVariableTable;

//static TSymbolTable* g_TopLevelUserVariableTable = CreateUserVariableTable(NULL);
//static TSymbolTable* currentTable = g_TopLevelUserVariableTable;

%}

%code requires
{
    //#include "myast.h"


    //typedef std::tuple<std::string, std::string, int> TokenTableRow;

    //extern std::vector <TokenTableRow> TokenTable;

    //void dumpTokenTable(std::string firstColName, std::string secondColName, std::string thirdColName);


}

/* The parsing context */
//%param { Simpl_driver& driver }

/*%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.filename;
};
*/

//%define parse.trace

%union
{
    NodeAST *astNode; // узел абс.синт. дерева
    double doubleValue;
    int intValue;
    QString *var; // переменная
    char opName[3]; // имя оператора
    QString *strValue; // строкова константа
    char other;
    SubexpressionValueTypeEnum type;
}

// Declare tokens.
%token       EOFILE 0   "end of file"
%token <doubleValue>   REALCONST  "floating point double precision"
%token <intValue>   INTCONST   "integer"
%token <var>        IDENTIFIER "name"
%token <strValue>   STRING     "string"
%token <opName>   RELOP      "relop"
%token <opName>   ADDUOP     "adduop"
%token <opName>   PLUS       "plus"
%token <opName>   MINUS      "minus"
%token <opName>   MULOP      "mulop"
%token <opName>   BOOLOP     "boolop"
%token       OPENBRACE  "{"
%token       CLOSEBRACE "}"
%token       OPENPAREN  "("
%token       CLOSEPAREN ")"
%token       OPENBRACKET    "["
%token       CLOSEBRACKET   "]"
%token       ASSIGN     "="
%token       SEMICOLON  ";"
%token       COLON      ":"
%token       COMA       ","
%token       DOT "."
// Control constructions.
%token       IF         "if"
%token       ELSE       "else"
%token       FOR        "for"
%token       WHILE      "while"
%token       DO         "do"
%token       SWITCH     "switch"
%token       CASE       "case"
%token       DEFAULT    "default"
%token       CONTINUE   "continue"
%token       BREAK      "break"
%token      GOTO       "goto"
// Types of data.
%token      INT        "int"
%token      DOUBLE     "double"
%token      FLOAT      "float"
%token      BOOL       "bool"
%token      CHAR       "char"
%token      SHORT      "short"
%token      ENUM       "enum"
%token      VOID       "void"
%token      STRUCT     "struct"
// Functions.
%token      SCAN       "scan"
%token      PRINT      "print"
//
%token      TRUE       "true"
%token      FALSE      "false"

%token      RETURN     "return"

/*%token IFX */

%type <astNode>  exp cond_stmt assignment statement compound_statement stmtlist stmtlist_tail prog declaration_number loop_stmt while_head switch_stmt
goto_stmt call_stmt descr_stmt case_stmt

%type <type> numeric_data_types

/* Priority */
%nonassoc IF
%nonassoc ELSE
%right ASSIGN
%left BOOLOP
%left RELOP
%left PLUS MINUS
%left MULOP
%left ADDUOP
%right UMINUS
%left OPENPAREN CLOSEPAREN OPENBRACKET CLOSEBRACKET

%start prog

/*%printer {
    yyoutput << $$;
} <*>*/

%destructor {
    delete $$;
} IDENTIFIER
%%

prog : stmtlist
{
    /*     if (driver.AST_dumping)
      { */
    printAST($1, 0);
    /*      } */
    freeAST($1);
    topLevelVariableTable->~SymbolsTable();
    /*   driver.result = 0;*/
};

stmtlist : statement stmtlist_tail
{
    if ($2 == NULL)
        $$ = $1;
    else
        $$ = createNodeAST(NT_List, "L", $1, $2);
};

stmtlist_tail : %empty   {
    $$ = NULL;
}
| stmtlist {
    $$ = $1;
};


statement : assignment
| cond_stmt
| declaration_number
| compound_statement
| loop_stmt
| switch_stmt
| goto_stmt
| call_stmt
| descr_stmt
| case_stmt
|BREAK SEMICOLON
{
    if (g_LoopNestingCounter <= 0)
        parsererror(errorList.at(ERROR_BREAK_NOT_INSIDE_LOOP));
    $$ = createControlFlowNode(NT_JumpStatement, NULL, NULL, NULL);
}
| error SEMICOLON // Restore after error
;


compound_statement :
OPENBRACE  { // {
    currentTable = currentTable->appendChildTable();
}
stmtlist
CLOSEBRACE { // }
    $$ = $3;
    currentTable->setHidden();
    currentTable = currentTable->getParent();
};

descr_stmt: numeric_data_types IDENTIFIER OPENPAREN exp CLOSEPAREN compound_statement {};

call_stmt: IDENTIFIER OPENPAREN exp CLOSEPAREN SEMICOLON {};

assignment : IDENTIFIER ASSIGN exp SEMICOLON
{    
    SymbolsTableRecord *var = currentTable->getVariableGlobal(*$1);
    if (var == NULL)
    {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$1));
        YYERROR;
    }
    else if ($3->valueType != var->valueType)
    {
        parsererror(errorList.at(WARNING_TYPES_INCOMPATIBLE).arg("assign"));
    }
    $$ = createAssignmentNode(var, $3);
}
|IDENTIFIER ASSIGN call_stmt {}
;

numeric_data_types : INT {
    $$ = typeInt;
}
| DOUBLE {
    $$ = typeDouble;
}
| FLOAT {
    $$ = typeFloat;
}
| BOOL {
    $$ = typeBool;
}
| CHAR {
    $$ = typeChar;
}
| SHORT {
    $$ = typeShort;
};

declaration_number : numeric_data_types IDENTIFIER SEMICOLON
{
    SymbolsTableRecord *var = currentTable->getVariableGlobal(*$2);
    if (var != NULL)
    {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(*$2));
        YYERROR;
    }
    else
    {

        var = currentTable->insertValue(*$2, $1, 0);
        if (var == NULL)
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
    }
    $$ = createAssignmentNode(var, createNumberNode(0));
}
| numeric_data_types IDENTIFIER ASSIGN exp SEMICOLON
{
    SymbolsTableRecord *var = currentTable->getVariableGlobal(*$2);
    if (var != NULL)
    {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(*$2));
        YYERROR;
    }
    else
    {

        var = currentTable->insertValue(*$2, $1, 0);
        if (var == NULL)
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
    }

    if ($4->valueType != var->valueType)
    {
        parsererror(errorList.at(WARNING_TYPES_INCOMPATIBLE).arg(*$2));
    }
    $$ = createAssignmentNode(var, $4);

}
| IDENTIFIER COLON {

}
;

goto_stmt: GOTO IDENTIFIER SEMICOLON {};

case_stmt: CASE IDENTIFIER COLON statement {}
    | CASE INTCONST COLON statement {}
    | DEFAULT COLON statement {}
;

switch_stmt: SWITCH OPENPAREN exp CLOSEPAREN statement CASE IDENTIFIER COLON statement {}
{};

print_stmt: PRINT OPENPAREN exp CLOSEPAREN SEMICOLON
{};

/*scan_stmt: SCAN OPENPAREN exp CLOSEPAREN
{};*/

cond_stmt: IF OPENPAREN exp CLOSEPAREN statement %prec IF
{
    $$ = createControlFlowNode(NT_IfStatement, $3, $5, NULL);
}
| IF OPENPAREN exp CLOSEPAREN statement ELSE statement
{
    $$ = createControlFlowNode(NT_IfStatement, $3, $5, $7);
};

loop_stmt : while_head statement
{
    $$ = createControlFlowNode(NT_WhileStatement, $1, $2, NULL);
    --g_LoopNestingCounter;
};

while_head : WHILE OPENPAREN exp CLOSEPAREN
{
    $$ = $3;
    ++g_LoopNestingCounter;
};

// Expression
exp : exp RELOP exp
{
    if ($1->valueType != $3->valueType)
    {
        parsererror(errorList.at(WARNING_TYPES_INCOMPATIBLE).arg($2));
        if ($1->valueType == typeInt)
            $$ = createNodeAST(NT_BinaryOperation, $2, createNodeAST(NT_UnaryOperation, "td", $1, NULL), $3);
        else
            $$ = createNodeAST(NT_BinaryOperation, $2, $1, createNodeAST(NT_UnaryOperation, "td", $3, NULL));
    }
    else
        $$ = createNodeAST(NT_BinaryOperation, $2, $1, $3);
}
| exp PLUS exp
{
    if ($1->valueType != $3->valueType)
    {
        parsererror(errorList.at(WARNING_TYPES_INCOMPATIBLE).arg($2));
        if ($1->valueType == typeInt)
            $$ = createNodeAST(NT_BinaryOperation, $2, createNodeAST(NT_UnaryOperation, "td", $1, NULL), $3);
        else
            $$ = createNodeAST(NT_BinaryOperation, $2, $1, createNodeAST(NT_UnaryOperation, "td", $3, NULL));
    }
    else
        $$ = createNodeAST(NT_BinaryOperation, $2, $1, $3);
}
| exp MINUS exp
{
    if ($1->valueType != $3->valueType)
    {
        parsererror(errorList.at(WARNING_TYPES_INCOMPATIBLE).arg($2));
        if ($1->valueType == typeInt)
            $$ = createNodeAST(NT_BinaryOperation, $2, createNodeAST(NT_UnaryOperation, "td", $1, NULL), $3);
        else
            $$ = createNodeAST(NT_BinaryOperation, $2, $1, createNodeAST(NT_UnaryOperation, "td", $3, NULL));
    }
    else
        $$ = createNodeAST(NT_BinaryOperation, "-", $1, $3);
}
| exp MULOP exp
{
    if ($1->valueType != $3->valueType)
    {
        parsererror(errorList.at(WARNING_TYPES_INCOMPATIBLE).arg($2));
        if ($1->valueType == typeInt)
            $$ = createNodeAST(NT_BinaryOperation, $2, createNodeAST(NT_UnaryOperation, "td", $1, NULL), $3);
        else
            $$ = createNodeAST(NT_BinaryOperation, $2, $1, createNodeAST(NT_UnaryOperation, "td", $3, NULL));
    }
    else
        $$ = createNodeAST(NT_BinaryOperation, $2, $1, $3);
}
| MINUS exp %prec UMINUS
{
    $$ = createNodeAST(NT_UnaryOperation, "-", $2, NULL);
}
| OPENPAREN exp CLOSEPAREN // ( exp )
{
    $$ = $2;
}
| OPENPAREN error CLOSEPAREN // Restore after error
{
    yyerrok;
}
| REALCONST
{
    $$ = createNumberNode($1);
}
| INTCONST
{
    $$ = createNumberNode($1);
}
| TRUE
{
    $$ = createNumberNode(1);
}
| FALSE
{
    $$ = createNumberNode(0);
}
| IDENTIFIER
{
    SymbolsTableRecord *var = currentTable->getVariableGlobal(*$1);

    if (var == NULL)
    {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$1));
        YYERROR;
                    //ErrorMessageVariableNotDeclared(*$1).c_str());
    }
    $$ = createReferenceNode(var);
}
;

%%
void yyerror(QString s) {

    printf ("Line %d: %s.\n", lno, s.toStdString().c_str());
    //std::cout << "EEK, parse error!  Message: " << s << std::endl;
    // might as well halt now:
    //exit(-1);
}

/*
int codegenBinary(FILE* outputFile, int operatorCode,
                  NodeAST* leftOperand, NodeAST* rightOperand, NodeAST* result)
{
    fprintf(outputFile, "\t$t%u\t:=\t", result->opValue);
    switch (leftOperand->nodetype)
    {
    case typeIdentifier:
        fprintf(outputFile, "%c", leftOperand->opValue);
        break;
    case typeList:
        fprintf(outputFile, "$t%d", leftOperand->opValue);
        break;
    case typeConst:
        fprintf(outputFile, "%g", leftOperand->opValue);
        break;
    }
    switch (operatorCode)
    {
    case ADDITION_OPERATOR:
        fprintf(outputFile, "+");
        break;
    case SUBTRACTION_OPERATOR:
        fprintf(outputFile, "-");
        break;
    case MULTIPLICATION_OPERATOR:
        fprintf(outputFile, "*");
        break;
    case DIVISION_OPERATOR:
        fprintf(outputFile, "/");
        break;
    }
    switch (rightOperand->nodetype)
    {
    case typeIdentifier:
        fprintf(outputFile, "%c", rightOperand->opValue);
        break;
    case typeList:
        fprintf(outputFile, "$t%d", rightOperand->opValue);
        break;
    case typeConst:
        fprintf(outputFile, "%g", rightOperand->opValue);
        break;
    }
    fprintf(outputFile, "\n");

    return 0;
}
int codegenUnary(FILE* outputFile, int operatorCode,
                 NodeAST* operand, NodeAST* result)
{
    if (operatorCode == OUTPUT_OPERATOR)
    {
        fprintf (outputFile, "\toutput\t");
    }
    else if (operatorCode == ASSIGN_OPERATOR)
    {
        fprintf(outputFile, "\t%c\t:=\t", result->opValue);
    }
    else
    {
        fprintf(outputFile, "\t$t%u\t:=\t", result->opValue);
        switch (operatorCode)
        {
        case UNPLUS_OPERATOR:
            fprintf(outputFile, "+");
            break;
        case NEGATION_OPERATOR:
            fprintf(outputFile, "-");
        }
    }
    switch (operand->nodetype)
    {
    case typeIdentifier:
        fprintf(outputFile, "%c", operand->opValue);
        break;
    case typeList:
        fprintf(outputFile, "$t%d", operand->opValue);
        break;
    case typeConst:
        fprintf(outputFile, "%g", operand->opValue);
        break;
    }
    fprintf(outputFile, "\n");

    return 0;
}
int codegenGoto(FILE* outputFile, int operatorCode,
                int labelREALCONST, NodeAST* optionalExpression)
{
    if (operatorCode != GOTO_OPERATOR)
    {
        if(operatorCode == IF_FALSE_GOTO_OPERATOR)
            fprintf(outputFile, "\tiffalse\t");
        else if(operatorCode == IF_TRUE_GOTO_OPERATOR)
            fprintf(outputFile, "\tiftrue\t");
        switch (optionalExpression->nodetype)
        {
        case typeIdentifier:
            fprintf(outputFile, "%c", optionalExpression->opValue);
            break;
        case typeList:
            fprintf(outputFile, "$t%d", optionalExpression->opValue);
            break;
        case typeConst:
            fprintf(outputFile, "%g", optionalExpression->opValue);
            break;
        default: break;
        }
    }
    fprintf(outputFile, "\tgoto\t$L%d", labelREALCONST);
    fprintf(outputFile, "\n");

    return 0;
}
int codegenLabel(FILE* outputFile, int labelREALCONST)
{
    fprintf (outputFile, "$L%d:", labelREALCONST);
    return 0;
}
static void PushLabelREALCONST(int labelREALCONST)
{
    Labels[g_LabelStackPointer] = labelREALCONST;
    ++g_LabelStackPointer;
}
static int PopLabelREALCONST(void)
{
    if (g_LabelStackPointer > 0)
    {
        --g_LabelStackPointer;
        return Labels[g_LabelStackPointer];
    }
    g_LabelStackPointer = 0;
    return -1;
}
static void EmptyLabels(void)
{
    g_LabelStackPointer = 0;
}

*/

/*int yyerror (std::string s) {
    printf ("Line %d: %s.\n", lno, s.c_str());
    //fprintf(stderr, "Line %d: %s.\n", lno, s);
    //g_ErrorStatus = !0;
    return !0;
}*/


