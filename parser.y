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
#include "astnode.h"
#include <QDebug>
}

%code provides {
    void parsererror(QString s);

    // Errors
    enum {  ERROR_DOUBLE_DECLARED,
            WARNING_DOUBLE_DECLARED,
            ERROR_NOT_DECLARED,
            ERROR_TYPES_INCOMPATIBLE,
            ERROR_BREAK_NOT_INSIDE_LOOP,
            ERROR_MEMORY_ALLOCATION,
            ERROR_UNRECOGNIZED_TOKEN,
            ERROR_UNFINISHED_STRING,
            ERROR_UNCLOSED_COMMENT,
            ERROR_COMPARSION_NOT_APPLICABLE,
            ERROR_COMPARSION_ON_DIFFERENCE_TYPES,
            ERROR_INCREMENT_WRONG_TYPE,
            ERROR_CANNOT_CONVERT,
            ERROR_DECLARATED_FUNCTION_NOT_GLOBAL,
            ERROR_CALL_UNDEFINED_FUNCTION};

    static QStringList errorList = QStringList() << "error: Variable %1 is already declared at this scope."
                                                 << "warning: Variable %1 is already declared."
                                                 << "error: Variable %1 was not declared."
                                                 << "error: can not convert %1 to %2"
                                                 << "error: 'break' not inside loop."
                                                 << "error: memory allocation or access error."
                                                 << "error: %1 - unrecognized token."
                                                 << "error: unfinished string."
                                                 << "error: nonclosed comment"
                                                 << "error: comparison oparation %1 not applicable for %2"
                                                 << "error: comparison oparation %1 not applicable on %2 and %3"
                                                 << "error: increment not applicable for %1 variable %2"
                                                 << "error: can't convert %1 to %2"
                                                 << "error: function declarated is not in the global scope"
                                                 << "error: call of undefined function %1";

}

%code requires
{

    //typedef std::tuple<std::string, std::string, int> TokenTableRow;

    //extern std::vector <TokenTableRow> TokenTable;

    //void dumpTokenTable(std::string firstColName, std::string secondColName, std::string thirdColName);

    bool isNumberType(ValueTypeEnum type);
    bool isNumberType(AbstractValueASTNode *node);

    AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
    AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
    AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value = NULL);
    //SymbolsTable* currentSwitch = NULL;

    SymbolsTableRecord *getVariableByName(QString name);

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


/* описание структуры синтаксического дерева */


/*
int g_tmpvar_upper_index = 0;
NodeAST* constants(double value);
NodeAST* idents (int index);
NodeAST* tmpvars (int tmp_index);*/
/* Генерация трехадресного кода */
/*int codegenBinary(FILE* outputFile, int operatorCode, NodeAST* leftOperand, NodeAST* rightOperand, NodeAST* result);
int codegenUnary(FILE* outputFile, int operatorCode, NodeAST* operand, NodeAST* result);
int codegenGoto(FILE* outputFile, int operatorCode,int labelREALCONST, NodeAST* optionalExpression);
int codegenLabel(FILE* outputFile, int labelREALCONST);*/

/* Обработка таблицы меток. Используется стековая организация */
/*
static int g_LastLabelREALCONST = 0;
static int g_LabelStackPointer = 0;
static int Labels[256];
static void PushLabelREALCONST(int);
static int PopLabelREALCONST(void);
static void EmptyLabels(void);
*/

extern int lno;
extern int parserlex();
//extern char g_outFileName[256]; /* Имя выходного файла */

//#undef yyerror
//#define yyerror driver.error

int g_LoopNestingCounter = 0;

static SymbolsTable* topLevelVariableTable = new SymbolsTable(NULL);
static SymbolsTable* currentTable = topLevelVariableTable;


%}



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
    AbstractASTNode *astNode; // узел абс.синт. дерева
    QString *variableName; // переменная
    char opName[3]; // имя оператора
    QVariant *value;
    ValueTypeEnum type;
}

// Declare tokens.
%token       EOFILE 0   "end of file"
%token <value>   REALCONST  "floating point double precision"
%token <value>   INTCONST   "integer"
%token <value>  CHARCONST "char"
%token <value>   STRING     "string"
%token <variableName>        IDENTIFIER "indentifer"
%token <opName>   RELOP      "relop"
%token <opName>   INCREMENT  "increment (decrement)"
%token <opName>   PLUS       "+"
%token <opName>   MINUS      "-"
%token <opName>   MULOP      "mulop"
%token      AND
%token      OR
%token      NOT
%token      XOR
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

%type <astNode>  program statement utterance statement_list statement_tail
numeric_declaration_statement numeric_declaration numeric_declaration_tail numeric_declaration_list
assignment assignment_statement compound_statement exp exp_list exp_tail
condition_statement
switch_statement case_statement case_list case_tail
loop_statement while_head break_statement for_head
goto_statement label_statement
function_call function_call_statement function_description_statement function_return_statement
print print_statement

%type <type> numeric_data_types function_types

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
%left INCREMENT
%left OPENPAREN CLOSEPAREN OPENBRACKET CLOSEBRACKET

%start program

/*%printer {
    yyoutput << $$;
} <*>*/

%destructor {
    delete $$;
} IDENTIFIER
%%

program : statement_list
{
    /*     if (driver.AST_dumping)
      { */
    //printAST($1, 0);
    $1->printNode(0);
    /*      } */
    //freeAST($1);
    $1->~AbstractASTNode();
    topLevelVariableTable->~SymbolsTable();
    /*   driver.result = 0;*/
};

statement_list : statement statement_tail
{
    if ($2 == NULL)
        $$ = $1;
    else
        $$ = new ListNode($1, $2);
};

statement_tail : %empty   {
    $$ = NULL;
}
| statement_list {
    $$ = $1;
};

utterance : print
{
    $$ = $1;
}
| exp
{
    $$ = $1;
}
| assignment
{
    $$ = $1;
}
| numeric_declaration
{
    $$ = $1;
}
| function_call
{
    $$ = $1;
};

statement : assignment_statement
| condition_statement
| numeric_declaration_statement
| compound_statement
| break_statement
| loop_statement
| switch_statement
| goto_statement
| label_statement
| function_call_statement
| function_description_statement
| function_return_statement
| print_statement
//| exp SEMICOLON
{
    //if ($1->getType() == NT_UnaryOperation)
}
| error SEMICOLON // Restore after error
{
    yyerrok;
};

compound_statement :
OPENBRACE  { // {
    currentTable = currentTable->appendChildTable();
}
statement_list
CLOSEBRACE { // }
    $$ = $3;
    currentTable->setHidden();
    currentTable = currentTable->getParent();
};


//Function --------------------------------------------------------------------------------
function_types : numeric_data_types {
    $$ = $1;
}
| VOID {
    $$ = typeVoid;
}

function_description_statement : numeric_data_types[type] IDENTIFIER[name] OPENPAREN {
    if(currentTable->getParent() != NULL) {
        parsererror(errorList.at(ERROR_DECLARATED_FUNCTION_NOT_GLOBAL));
        YYERROR;
    }
    currentTable = currentTable->appendChildTable();


}
    numeric_declaration_list[params] CLOSEPAREN compound_statement[body] {
        currentTable->setHidden();
        SymbolsTable *paramsTable = currentTable;
        currentTable = currentTable->getParent();
        SymbolsTableRecord *function = currentTable->insertValue(*$name, $type, 0, paramsTable);

        $$ = new FunctionDeclareNode(function, $params, $body);
        qDebug() << "Function descripted";
    };

function_call_statement : function_call SEMICOLON
{

    $$ = $1;
};
function_call : IDENTIFIER[id] OPENPAREN exp_list[explist] CLOSEPAREN
{
    SymbolsTableRecord *function = getVariableByName(*$id);
    if (function != NULL) {
        if (function->params != NULL) {
            $$ = new FunctionCallNode(function, $explist);
        }
        else {
            parsererror(errorList.at(ERROR_CALL_UNDEFINED_FUNCTION).arg(*$id));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        parsererror(errorList.at(ERROR_CALL_UNDEFINED_FUNCTION).arg(*$id));
        $$ = NULL;
        YYERROR;
    }
};

function_return_statement : RETURN exp SEMICOLON {
    $$ = new FunctionReturnNode($exp);
}
// Numeric declaration ------------------------------------------------------------
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

numeric_declaration_statement : numeric_declaration SEMICOLON
{
    $$ = $1;
};

numeric_declaration_list : numeric_declaration numeric_declaration_tail[tail]
{
    if ($tail == NULL) {
        $$ = $1;
    }
    else {
        if($1 == NULL)
            $$ = NULL;
        else
            $$ = new ListNode($1, $tail);
    }
};

numeric_declaration_tail : %empty {
    $$ = NULL;
}
| numeric_declaration_list COMA {
    $$ = $1;
}
| numeric_declaration_list {
    $$ = $1;
};

numeric_declaration : numeric_data_types IDENTIFIER
{
    AbstractASTNode *var = numericDeclaration($1, *$2);
    if(var == NULL)
        YYERROR;
    $$ = var;
}
| numeric_data_types IDENTIFIER ASSIGN exp
{
    AbstractASTNode *var = numericDeclaration($1, *$2, (AbstractValueASTNode *)$4);
    if(var == NULL)
        YYERROR;
    $$ = var;
}
| %empty
{
    $$ = NULL;
};

// GoTo Break and Label ------------------------------------------------------------------
label_statement : IDENTIFIER COLON {
    SymbolsTableRecord *label = NULL;
    if (currentTable->containsGlobal(*$1)) {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(*$1));
    }
    else {
        label = currentTable->insertValue(*$1, typeLabel, *$1);
    }
    $$ = new LabelNode(label);
}

goto_statement : GOTO IDENTIFIER SEMICOLON {
    if (currentTable->containsGlobal(*$2)) {
            $$ = new GoToNode(*$2);
    }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$2));
        $$ = NULL;
    }
};

break_statement : BREAK SEMICOLON
{
    if (g_LoopNestingCounter <= 0)
        parsererror(errorList.at(ERROR_BREAK_NOT_INSIDE_LOOP));
    $$ = new GoToNode("EndOfLoop");//NULL;//createControlFlowNode(NT_JumpStatement, NULL, NULL, NULL);
}


//Switch - case -------------------------------------------------------------------
case_list : case_statement case_tail
{
    if ($2 == NULL) {
        $$ = $1;
    }
    else {
        $$ = new ListNode($1, $2);
    }
};

case_tail : %empty {
    $$ = NULL;
}
| case_list {
    $$ = $1;
};


case_statement : CASE IDENTIFIER COLON statement_list {
    SymbolsTableRecord *var = getVariableByName(*$2);
    if (var != NULL) {

        $$ = new CaseNode(new ReferenceNode(var), $4);
    }
    else {
        $$ = NULL;
    }
}
| CASE INTCONST COLON statement_list {
    $$ = new CaseNode(new ValueNode(typeInt, $2->toInt()), $4);
}
| DEFAULT COLON statement_list {
    $$ = new CaseNode(NULL, $3);
}
;

switch_statement : SWITCH OPENPAREN exp CLOSEPAREN OPENBRACE case_list CLOSEBRACE {}
{

    $$ = new SwitchNode($3, $6);

};

//Print and scan-------------------------------------------------------------------
print_statement : print SEMICOLON
{
    $$ = $1;
};

print : PRINT OPENPAREN exp CLOSEPAREN
{
    $$ = new PrintNode($3);
};

/*scan_stmt: SCAN OPENPAREN exp CLOSEPAREN
{};*/

condition_statement : IF OPENPAREN exp CLOSEPAREN statement %prec IF
{
    $$ = new IfNode($3, $5, NULL);
}
| IF OPENPAREN exp CLOSEPAREN statement ELSE statement
{
    $$ = new IfNode($3, $5, $7);
};

//Loops -------------------------------------------------------------------
loop_statement : while_head statement
{
    ((WhileNode *)$1)->setBody($2);
    $$ = $1;
    --g_LoopNestingCounter;
}
| for_head statement
{
    ((ForNode *)$1)->setBody($2);
    $$ = $1;
    --g_LoopNestingCounter;

    currentTable->setHidden();
    currentTable = currentTable->getParent();
}
| do_head statement while_head
{
    ((WhileNode *)$while_head)->setBody($2);
    ((WhileNode *)$while_head)->setIsDoWhile(true);
    $$ = $3;
    --g_LoopNestingCounter;
}
;

while_head : WHILE OPENPAREN exp[condition] CLOSEPAREN
{
    AbstractValueASTNode *cond = NULL;
    if ($condition != NULL) {
        cond = (AbstractValueASTNode *)$condition;
        if (cond->getType() != typeBool)
            cond = new UnaryNode(UnarToBool, cond);
    }

    $$ = new WhileNode(cond);
    ++g_LoopNestingCounter;
};

for_head : FOR
{
    currentTable = currentTable->appendChildTable();
}
    OPENPAREN utterance[init] SEMICOLON exp[condition] SEMICOLON utterance[increment] CLOSEPAREN
{
    AbstractValueASTNode *cond = NULL;
    if ($condition != NULL) {
        cond = (AbstractValueASTNode *)$condition;
        if (cond->getType() != typeBool)
            cond = new UnaryNode(UnarToBool, cond);
    }

    $$ = new ForNode($init, cond, $increment);
    ++g_LoopNestingCounter;
};

do_head : DO
{
    ++g_LoopNestingCounter;
}

// Assigments ---------------------------------------------------------------
    assignment_statement : assignment SEMICOLON
    {
        $$ = $1;
    };

    assignment : IDENTIFIER ASSIGN exp
    {
        SymbolsTableRecord *var = getVariableByName(*$1);
        if (!isNumberType((AbstractValueASTNode *)$3))
        {
            parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE).arg(typeName.at(((AbstractValueASTNode *)$3)->getType())).arg(typeName.at(var->valueType)));
            YYERROR;
        }
        else
        {
            if (((AbstractValueASTNode *)$3)->getType() != var->valueType) {
                switch(var->valueType) {
                case typeBool:
                    $$ = new AssignmentNode(var, new UnaryNode(UnarToBool, $3));
                    break;
                case typeInt:
                    $$ = new AssignmentNode(var, new UnaryNode(UnarToInt, $3));
                    break;
                case typeDouble:
                    $$ = new AssignmentNode(var, new UnaryNode(UnarToDouble, $3));
                    break;
                }
            }
            else {
                $$ = new AssignmentNode(var, $3);
            }
        }
    };

// Expressions --------------------------------------------------------------
exp_list : exp exp_tail[tail]
{
    if ($tail == NULL) {
        $$ = $1;
    }
    else {
        $$ = new ListNode($1, $tail);
    }
};

exp_tail : %empty {
    $$ = NULL;
}
| exp_list COMA {
    $$ = $1;
}
| exp_list {
    $$ = $1;
};

exp : exp[left] RELOP exp[right]
{
    // Relation exp

    AbstractValueASTNode *left = ((AbstractValueASTNode *)$left);
    AbstractValueASTNode *right = ((AbstractValueASTNode *)$right);

    if (isNumberType(left) && isNumberType(right)) { // Type of variables is number.
        if (left->getType() != right->getType()) {// If exp have diffrence type.
            if (left->getType() == typeDouble || right->getType() == typeDouble) { // If left or right expression have type Double add to double
                if (left->getType() == typeDouble) {
                    $$ = new BinarNode($left, new UnaryNode(UnarToDouble, $right), $2, typeBool);
                }
                else {
                    $$ = new BinarNode(new UnaryNode(UnarToDouble, $left), $right, $2, typeBool);
                }
            }
            else { // if left or right expression have diffrent type of int converting to int
                if (left->getType() != typeInt)
                    left = new UnaryNode(UnarToInt, $left);
                if (right->getType() != typeInt)
                    right = new UnaryNode(UnarToInt, $right);

                $$ = new BinarNode($left, $right, $2, typeBool);
            }
        }
        else
            $$ = new BinarNode($left, $right, $2, typeBool);
    }
    else {
        if (left->getType() == right->getType()) {
            if ($2 == "==")
                $$ = new BinarNode($left, $right, $2, typeBool);
            else
                parsererror(errorList.at(ERROR_COMPARSION_NOT_APPLICABLE).arg($2).arg(left->getType()));
        }
        parsererror(errorList.at(ERROR_COMPARSION_ON_DIFFERENCE_TYPES).arg($2).arg(left->getType()).arg(right->getType()));
    }
}
| exp[left] AND exp[right]
{
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "&", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] AND AND exp[right]
{
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "&&", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] OR exp[right]
{
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "|", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] OR OR exp[right]
{
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "||", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] XOR exp[right]
{
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "^", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| NOT exp
{
    AbstractValueASTNode *var = (AbstractValueASTNode *)$2;
    if (isNumberType(var)) {
        if (var->getType() != typeBool) {
            var = new UnaryNode(UnarToBool, var);
        }
        $$ = new UnaryNode(UnarNot, var);
    }
    else {
        parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                    .arg(typeName.at(var->getType()))
                    .arg(typeName.at(typeBool)));
        $$ = NULL;
        YYERROR;
    }
}
| exp PLUS exp
{
    // PLUS exp
    AbstractASTNode *p = binarMathNode((AbstractValueASTNode *)$1, $2, (AbstractValueASTNode *)$3);
    if (p == NULL)
        YYERROR;

    $$ = p;
}
| exp MINUS exp
{
    // MINUS exp
    AbstractASTNode *p = binarMathNode((AbstractValueASTNode *)$1, $2, (AbstractValueASTNode *)$3);
    if (p == NULL)
        YYERROR;

    $$ = p;
}
| exp MULOP exp
{
    // MULOP exp
    AbstractASTNode *p = binarMathNode((AbstractValueASTNode *)$1, $2, (AbstractValueASTNode *)$3);
    if (p == NULL)
        YYERROR;

    $$ = p;
}
| MINUS exp %prec UMINUS
{
    $$ = new UnaryNode(UnarMinus, $2);
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
    $$ = new ValueNode(typeDouble, $1->toDouble());
}
| INTCONST
{
    $$ = new ValueNode(typeInt, $1->toInt());
}
| CHARCONST
{
    $$ = new ValueNode(typeChar, $1->toChar());
}
| TRUE
{
    $$ = new ValueNode(typeBool, 1);
}
| FALSE
{
    $$ = new ValueNode(typeBool, 0);
}
| IDENTIFIER
{
    SymbolsTableRecord *var = getVariableByName(*$1);
    $$ = new ReferenceNode(var);
}
| function_call
{
    $$ = $1;
}
| %empty
{
    $$ = NULL;
}

/*
 * Not in 2016.
 * | IDENTIFIER INCREMENT
{
    SymbolsTableRecord *var = getVariableByName(*$1);

    if (isNumberType(var->valueType))
        $$ = createNodeAST(NT_UnaryOperation, $2, createReferenceNode(var), NULL);
    else
        parsererror(errorList.at(ERROR_INCREMENT_WRONG_TYPE).arg(typeName.at(var->valueType)).arg(*$1));

}
*/
;

%%
void yyerror(QString s) {

    printf ("Line %d: %s.\n", lno, s.toStdString().c_str());
    //std::cout << "EEK, parse error!  Message: " << s << std::endl;
    // might as well halt now:
    //exit(-1);
}

SymbolsTableRecord *getVariableByName(QString name) {
    SymbolsTableRecord *var = currentTable->getVariableGlobal(name);

    if (var == NULL)
    {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(name));
        //YYERROR;
    }
    return var;
}

bool isNumberType(ValueTypeEnum type)
{
    if (type == typeInt ||
            type == typeBool ||
            type == typeDouble ||
            type == typeFloat ||
            type == typeShort ||
            type == typeChar)
        return true;

    return false;
}

bool isNumberType(AbstractValueASTNode *node)
{
    return isNumberType(((AbstractValueASTNode *)node)->getType());
}

AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if (isNumberType(left) && isNumberType(right)) {
        if (left->getType() != right->getType())
        {
            if (left->getType() == typeInt)
                return new BinarNode(new UnaryNode(UnarToDouble, left), right, operation);
            else
                return new BinarNode(left, new UnaryNode(UnarToDouble, right), operation);
        }
        else
            return new BinarNode(left, right, operation);
    }
    else {
        parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE).arg(typeName.at(left->getType())).arg(typeName.at(right->getType())));
        return NULL;
    }
}

AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if(isNumberType(left) && isNumberType(right)) {
        if (operation == "&&" || operation == "||") {
        if (left->getType() != typeBool)
            left = new UnaryNode(UnarToBool, left);
        if (right->getType() != typeBool)
            right = new UnaryNode(UnarToBool, right);
        }

        return new BinarNode(left, right, operation, typeBool);
    }
    else {
        if(!isNumberType(left))
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(left->getType()))
                        .arg(typeName.at(typeBool)));
        if(!isNumberType(right))
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(right->getType()))
                        .arg(typeName.at(typeBool)));
        return NULL;
    }
}


AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value)
{
    //numeric_declaration without assign
    SymbolsTableRecord *var = NULL;
    if (!currentTable->contains(name)) {
        var = currentTable->getVariableGlobal(name);
        if (var != NULL)
        {
            parsererror(errorList.at(WARNING_DOUBLE_DECLARED).arg(name));
        }

        var = currentTable->insertValue(name, type, 0);
        if (var == NULL) {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            return NULL;
        }

        if (value != NULL) {
            if (!isNumberType(value))
            {
                parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE)
                            .arg(typeName.at(value->getType()))
                            .arg(typeName.at(var->valueType)));
                return NULL;
            }
            else {
                if (value->getType() != var->valueType) {
                    switch(var->valueType) {
                    case typeBool :
                        value = new UnaryNode(UnarToBool, value);
                        break;
                    case typeFloat :
                    case typeDouble :
                        value = new UnaryNode(UnarToDouble, value);
                        break;
                    case typeInt :
                    default :
                        value = new UnaryNode(UnarToInt, value);
                    }
                }

                return new AssignmentNode(var, value);
            }
        }
        else {
                return new AssignmentNode(var, new ValueNode(var->valueType, 0));
        }
    }
    else {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(name));
        return NULL;
    }
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
        if (operatorCode == IF_FALSE_GOTO_OPERATOR)
            fprintf(outputFile, "\tiffalse\t");
        else if (operatorCode == IF_TRUE_GOTO_OPERATOR)
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


