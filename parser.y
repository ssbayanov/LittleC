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
            ERROR_CALL_UNDEFINED_FUNCTION,
            ERROR_UNKNOWN_TYPE,
            WARNING_CONVERTING_TYPES};

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
                                                 << "error: call of undefined function %1"
                                                 << "error: unknown type %1"
                                                 << "warning: during conversion %1 to %2 can be errors";

}

%code requires
{

    //typedef std::tuple<std::string, std::string, int> TokenTableRow;

    //extern std::vector <TokenTableRow> TokenTable;

    //void dumpTokenTable(std::string firstColName, std::string secondColName, std::string thirdColName);

    bool isNumericType(ValueTypeEnum type);
    bool isNumericType(AbstractValueASTNode *node);

    AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
    AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
    AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value = NULL);
    AbstractASTNode *numericAssign(SymbolsTableRecord *var, AbstractValueASTNode *value = NULL);
    AbstractValueASTNode *convert(AbstractValueASTNode *what, ValueTypeEnum to);
    int getSizeType(ValueTypeEnum type);
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
%token <value>   INTCONST   "integer constant"
%token <value>  CHARCONST "char constant"
%token <value>   STRING     "string"
%token <variableName>        IDENTIFIER "indentifer"
%token <opName>   RELOP      "relation operator"
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
declaration_statement declaration declaration_list_element declaration_list
assignment assignment_statement compound_statement exp exp_list exp_list_element
condition_statement
switch_statement case_statement case_list case_tail
loop_statement while_head break_statement for_head
goto_statement label_statement
function_call function_call_statement function_description_statement function_return_statement
print print_statement

%type <type> data_types

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
//%right data_types function_types
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
    $1->printNode(0);
    /*      } */
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
| declaration
{
    $$ = $1;
}
| function_call
{
    $$ = $1;
};

statement : assignment_statement
| condition_statement
| declaration_statement
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
/*| exp SEMICOLON
{
    //if ($1->getType() == NT_UnaryOperation)
}*/
| error SEMICOLON // Restore after error
{
    yyerrok;
    $$ = NULL;
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

data_types : INT {
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
}
| VOID {
    $$ = typeVoid;
};

//Function --------------------------------------------------------------------------------
function_description_statement : data_types[type] IDENTIFIER[name] OPENPAREN {
    if(currentTable->getParent() != NULL) {
        parsererror(errorList.at(ERROR_DECLARATED_FUNCTION_NOT_GLOBAL));
        YYERROR;
    }
    currentTable = currentTable->appendChildTable();


}
    declaration_list[params] CLOSEPAREN compound_statement[body] {
        currentTable->setHidden();
        SymbolsTable *paramsTable = currentTable;
        currentTable = currentTable->getParent();
        SymbolsTableRecord *function = currentTable->insertValue(*$name, $type, 0, paramsTable);

        $$ = new FunctionDeclareNode(function, $params, $body);
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
            //if()
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


declaration_statement : declaration SEMICOLON
{
    $$ = $1;
};

declaration_list : declaration_list_element declaration_list_element[tail]
{
    if ($tail == NULL) {
        $$ = $1;
    }
    else {
        $$ = new ListNode($1, $tail);
    }
};

declaration_list_element : %empty {
    $$ = NULL;
}
| declaration COMA {
    $$ = $1;
}
| declaration {
    $$ = $1;
};

declaration : data_types[type] IDENTIFIER
{
    if (isNumericType( $type )) {
        AbstractASTNode *var = numericDeclaration($1, *$2);
        if(var == NULL)
            YYERROR;
        $$ = var; }
    else {
        $$ = NULL;
    }
}
| data_types IDENTIFIER ASSIGN exp
{
    AbstractASTNode *var = numericDeclaration($1, *$2, (AbstractValueASTNode *)$4);
    if(var == NULL)
        YYERROR;
    $$ = var;
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
    if (g_LoopNestingCounter <= 0) {
        parsererror(errorList.at(ERROR_BREAK_NOT_INSIDE_LOOP));
        $$ = NULL;
    }
    else {
        $$ = new GoToNode("EndOfLoop");//NULL;//createControlFlowNode(NT_JumpStatement, NULL, NULL, NULL);
    }
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
};

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

assignment : IDENTIFIER[name] ASSIGN exp[value]
{
    SymbolsTableRecord *var = currentTable->getVariableGlobal(*$name);
    if(var != NULL) {
        AbstractASTNode *node = numericAssign(var, (AbstractValueASTNode *)$value);
        if(node == NULL)
            YYERROR;
        $$ = node; }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$name));
        $$ = NULL;
        YYERROR;
    }
};

// Expressions --------------------------------------------------------------
exp_list : exp_list_element exp_list_element[tail]
{
    if ($tail == NULL) {
        $$ = $1;
    }
    else {
        $$ = new ListNode($1, $tail);
    }
};

exp_list_element : %empty {
    $$ = NULL;
}
| exp COMA {
    $$ = $1;
}
| exp {
    $$ = $1;
};

exp : exp[left] RELOP exp[right]
{
    // Relation exp

    AbstractValueASTNode *left = ((AbstractValueASTNode *)$left);
    AbstractValueASTNode *right = ((AbstractValueASTNode *)$right);

    if (isNumericType(left) && isNumericType(right)) { // Type of variables is number.
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
            if (QString("==").contains(*$2))
                $$ = new BinarNode($left, $right, $2, typeBool);
            else {
                $$ = NULL;
                parsererror(errorList.at(ERROR_COMPARSION_NOT_APPLICABLE).arg($2).arg(left->getType()));
                YYERROR;
            }
        }
        else {
            $$ = NULL;
            parsererror(errorList.at(ERROR_COMPARSION_ON_DIFFERENCE_TYPES).arg($2).arg(left->getType()).arg(right->getType()));
            YYERROR;
        }
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
    if (isNumericType(var)) {
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

/*
 * Not in 2016.
 * | IDENTIFIER INCREMENT
{
    SymbolsTableRecord *var = getVariableByName(*$1);

    if (isNumericType(var->valueType))
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

bool isNumericType(ValueTypeEnum type)
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

bool isNumericType(AbstractValueASTNode *node)
{
    return isNumericType(((AbstractValueASTNode *)node)->getType());
}

AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if (isNumericType(left) && isNumericType(right)) {
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
    if(isNumericType(left) && isNumericType(right)) {
        if (operation == "&&" || operation == "||") {
        if (left->getType() != typeBool)
            left = new UnaryNode(UnarToBool, left);
        if (right->getType() != typeBool)
            right = new UnaryNode(UnarToBool, right);
        }

        return new BinarNode(left, right, operation, typeBool);
    }
    else {
        if(!isNumericType(left))
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(left->getType()))
                        .arg(typeName.at(typeBool)));
        if(!isNumericType(right))
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(right->getType()))
                        .arg(typeName.at(typeBool)));
        return NULL;
    }
}


AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value)
{
    if (!currentTable->contains(name)) {
        if (currentTable->getVariableGlobal(name) != NULL)
        {
            parsererror(errorList.at(WARNING_DOUBLE_DECLARED).arg(name));
        }


        SymbolsTableRecord *var = currentTable->insertValue(name, type, 0);
        if(var == NULL) {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            return NULL;
        }
        return numericAssign(var, value);
    }
    else {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(name));
        return NULL;
    }
}

AbstractASTNode *numericAssign(SymbolsTableRecord *var, AbstractValueASTNode *value)
{

    if (var == NULL) {
        parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
        return NULL;
    }

    if (value != NULL) {
        if (!isNumericType(value))
        {
            parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE)
                        .arg(typeName.at(value->getType()))
                        .arg(typeName.at(var->valueType)));
            return NULL;
        }
        else {
            if (value->getType() != var->valueType) {
                value = convert(value, var->valueType);
            }

            return new AssignmentNode(var, value);
        }
    }
    else {
            return new AssignmentNode(var, new ValueNode(var->valueType, 0));
    }
}

AbstractValueASTNode *convert(AbstractValueASTNode *what, ValueTypeEnum to)
{
    ValueTypeEnum whatType = what->getType();
    if(isNumericType(whatType)) {
        if(isNumericType(to)) {
            switch(to){
            case typeBool:
                return new UnaryNode(UnarToBool, what);
            case typeChar:
                if(getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                               .arg(typeName.at(whatType))
                               .arg(typeName.at(to)));
                return new UnaryNode(UnarToChar, what);
            case typeDouble:
                return new UnaryNode(UnarToDouble, what);;
            case typeFloat:
                if(getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                               .arg(typeName.at(whatType))
                               .arg(typeName.at(to)));
                return new UnaryNode(UnarToFloat, what);
            case typeInt:
                if(getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                               .arg(typeName.at(whatType))
                               .arg(typeName.at(to)));
                return new UnaryNode(UnarToInt, what);
            case typeShort:
                if(getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                               .arg(typeName.at(whatType))
                               .arg(typeName.at(to)));
                return new UnaryNode(UnarToShort, what);
            default:
                parsererror(errorList.at(ERROR_UNKNOWN_TYPE).arg(to));
                return NULL;
            }
        }
    }
    parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE)
                .arg(typeName.at(what->getType()))
                .arg(typeName.at(to)));
    return NULL;

}

int getSizeType(ValueTypeEnum type)
{
    switch(type) {
    case typeVoid:
        return 0;
    case typeBool:
    case typeChar:
        return 1;
    case typeShort:
        return 2;
    case typeFloat:
    case typeInt:
        return 4;
    case typeDouble:
        return 8;
    default:
        parsererror(errorList.at(ERROR_UNKNOWN_TYPE).arg(type));
        return 0;
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


