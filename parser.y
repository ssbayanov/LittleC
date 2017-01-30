%code requires { //code insert in top parser_yacc.h

#include <iostream>
#include <QString>
#include <QStringList>

#include "symbolstable/symbolstable.h"
#include "abstracttree/ast.h"
#include <QDebug>
}

%code provides {
    void parsererror(QString s);

    extern QTextStream errorStream;
    extern QTextStream cout;

    extern bool printTree;
    extern bool printWarnings;
    extern bool printErrors;

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
            ERROR_CALL_UNDEFINED_FUNCTION_WITH_PARAMS,
            ERROR_UNKNOWN_TYPE,
            WARNING_CONVERTING_TYPES,
            ERROR_CANNOT_USE_AS_INDEX,
            ERROR_VARIABLE_IS_NOT_ARRAY,
            ERROR_SCAN_STRING,
            ERROR_PARAMS_FUNCTION};

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
                                                 << "error: call of undefined function %1()"
                                                 << "error: call of undefined function %1(%2);"
                                                 << "error: unknown type %1"
                                                 << "warning: during conversion %1 to %2 can be errors"
                                                 << "error: can't use %1 type as index of array"
                                                 << "error: variable %1 is not a array"
                                                 << "error: scan string is not valid"
                                                 << "error: params of call function %1 is not valid";


    bool isNumericType(ValueTypeEnum type);
    bool isNumericType(AbstractValueASTNode *node);

    AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
    AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
    AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value = NULL);
    AbstractASTNode *numericAssign(AbstractSymbolTableRecord *var, AbstractValueASTNode *value = NULL);
    AbstractASTNode *arrayAssign(AbstractSymbolTableRecord *var, AbstractValueASTNode *index, AbstractValueASTNode *value);
    AbstractValueASTNode *convert(AbstractValueASTNode *what, ValueTypeEnum to);
    AbstractASTNode *validatedParams(FunctionTableRecord *function, AbstractASTNode *paramsNode);
    int getSizeType(ValueTypeEnum type);

    AbstractSymbolTableRecord *getVariableByName(QString name);
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

extern int lno;
extern int parserlex();

int g_LoopNestingCounter = 0;
int enumCounter = 0;

static SymbolsTable* topLevelVariableTable = new SymbolsTable(NULL);
static SymbolsTable* currentTable = topLevelVariableTable;
%}

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
declaration declaration_list_element declaration_list enum_decloration_statement
assignment compound_statement exp exp_list identifier_list
array_declaration array_declaration_statement
condition_statement
switch_statement case_statement case_list case_tail
loop_statement while_head break_statement for_head
goto_statement label_statement
function_call function_description_statement function_return_statement
print scan

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

%destructor {
    delete $$;
} IDENTIFIER
%%

program : statement_list
{
    /*     if (driver.AST_dumping)
      { */
    if(printTree)
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
| assignment
| declaration
| function_call;

statement : condition_statement
| compound_statement
| break_statement
| loop_statement
| switch_statement
| goto_statement
| label_statement
| array_declaration_statement
| function_description_statement
| function_return_statement
| enum_decloration_statement
| utterance SEMICOLON {
    $$ = $1;
}
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
    if (currentTable->getParent() != NULL) {
        parsererror(errorList.at(ERROR_DECLARATED_FUNCTION_NOT_GLOBAL));
        YYERROR;
    }
    currentTable = currentTable->appendChildTable();


}
declaration_list[params] CLOSEPAREN compound_statement[body] {
    currentTable->setHidden();
    SymbolsTable *paramsTable = currentTable;
    currentTable = currentTable->getParent();
    AbstractSymbolTableRecord *function = currentTable->insertFunction(*$name, $type, paramsTable);

    $$ = new FunctionDeclareNode(function, $params, $body);
};

function_call : IDENTIFIER[id] OPENPAREN exp_list[params] CLOSEPAREN
{
    AbstractSymbolTableRecord *function = getVariableByName(*$id);
    if (function != NULL) {
        if (function->isFunction()) {
            AbstractASTNode *validParams = validatedParams(((FunctionTableRecord *) function), $params);
            if (validParams != NULL) {
                $$ = new FunctionCallNode(function, validParams);
            }
            else {
                $$ = NULL;
                YYERROR;
            }
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
// Arrays -------------------------------------------------------------------------
array_declaration_statement : array_declaration SEMICOLON {
    $$ = $1;
};

array_declaration : data_types[type] IDENTIFIER[name] OPENBRACKET exp[values] CLOSEBRACKET {
    AbstractSymbolTableRecord *array = currentTable->insertArray(*$name, $type);
    if (array != NULL) {
        $$ = new ArrayDeclareNode(array, $values);
    }
    else {
        parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
        $$ = NULL;
        YYERROR;
    }

}
| data_types[type] IDENTIFIER[name] OPENBRACKET CLOSEBRACKET ASSIGN OPENBRACE exp_list[values] CLOSEBRACE {
    AbstractSymbolTableRecord *array = currentTable->insertArray(*$name, $type);
    if (array != NULL) {
        $$ = new ArrayDeclareNode(array, $values);
    }
    else {
        parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
        $$ = NULL;
        YYERROR;
    }
}
| data_types[type] IDENTIFIER[name] OPENBRACKET CLOSEBRACKET ASSIGN STRING[value] {
    if ($type == typeChar) {
        AbstractSymbolTableRecord *array = currentTable->insertArray(*$name, $type);
        if (array != NULL) {
            $$ = new ArrayDeclareNode(array, new ValueNode(typeString, *$value));
        }
        else {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        parsererror(errorList.at(ERROR_CANNOT_CONVERT).arg(typeName.at(typeChar)).arg(typeName.at($type)));
        $$ = NULL;
        YYERROR;
    }
};

// Numeric declaration ------------------------------------------------------------
enum_decloration_statement : ENUM { enumCounter = 0; } OPENBRACE identifier_list[values] CLOSEBRACE SEMICOLON{
    $$ = $values;
}


declaration_list : declaration_list_element declaration_list_element[tail] {
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
        if (var == NULL)
            YYERROR;
        $$ = var; }
    else {
        $$ = NULL;
    }
}
| data_types IDENTIFIER ASSIGN exp
{
    AbstractASTNode *var = numericDeclaration($1, *$2, (AbstractValueASTNode *)$4);
    if (var == NULL)
        YYERROR;
    $$ = var;
};

// GoTo Break and Label ------------------------------------------------------------------
label_statement : IDENTIFIER COLON {
    AbstractSymbolTableRecord *label = NULL;
    if (currentTable->containsGlobal(*$1)) {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(*$1));
    }
    else {
        label = currentTable->insertVariable(*$1, typeLabel, *$1);
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

break_statement : BREAK SEMICOLON {
    if (g_LoopNestingCounter <= 0) {
        parsererror(errorList.at(ERROR_BREAK_NOT_INSIDE_LOOP));
        $$ = NULL;
    }
    else {
        $$ = new GoToNode("EndOfLoop");
    }
}


//Switch - case -------------------------------------------------------------------
case_list : case_statement case_tail {
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
    AbstractSymbolTableRecord *var = getVariableByName(*$2);
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

switch_statement : SWITCH OPENPAREN exp CLOSEPAREN OPENBRACE case_list CLOSEBRACE {
    $$ = new SwitchNode($3, $6);
};

//Print and scan-------------------------------------------------------------------
print : PRINT OPENPAREN exp CLOSEPAREN {
    $$ = new PrintNode($3);
};

scan: SCAN OPENPAREN STRING[typeString] CLOSEPAREN {
    ValueTypeEnum scanType = typeVoid;
    if($typeString->toString().at(0) == '%') {
        switch($typeString->toString().at(1).toLatin1()) {
        case 'd':
        case 'i': scanType = typeInt; break;
        case 'e':
        case 'f':
        case 'g': scanType = typeInt; break;
        case 's': scanType = typeString; break;
        case 'c': scanType = typeChar; break;
        default: parsererror(errorList.at(ERROR_SCAN_STRING));
            $$ = NULL;
            YYERROR;
        }
        if(scanType != typeVoid)
            $$ = new ScanNode(scanType);
    }
    else {
        parsererror(errorList.at(ERROR_SCAN_STRING));
        $$ = NULL;
        YYERROR;
    }

};

condition_statement : IF OPENPAREN exp CLOSEPAREN statement %prec IF {
    $$ = new IfNode($3, $5, NULL);
}
| IF OPENPAREN exp CLOSEPAREN statement ELSE statement {
    $$ = new IfNode($3, $5, $7);
};

//Loops -------------------------------------------------------------------
loop_statement : while_head statement {
    ((WhileNode *)$1)->setBody($2);
    $$ = $1;
    --g_LoopNestingCounter;
}
| for_head statement {
    ((ForNode *)$1)->setBody($2);
    $$ = $1;
    --g_LoopNestingCounter;

    currentTable->setHidden();
    currentTable = currentTable->getParent();
}
| do_head statement while_head SEMICOLON{
    ((WhileNode *)$while_head)->setBody($2);
    ((WhileNode *)$while_head)->setIsDoWhile(true);
    $$ = $3;
    --g_LoopNestingCounter;
}
;

while_head : WHILE OPENPAREN exp[condition] CLOSEPAREN {
    AbstractValueASTNode *cond = NULL;
    if ($condition != NULL) {
        cond = (AbstractValueASTNode *)$condition;
        if (cond->getValueType() != typeBool)
            cond = new UnaryNode(UnarToBool, cond);
    }

    $$ = new WhileNode(cond);
    ++g_LoopNestingCounter;
};

for_head : FOR {
    currentTable = currentTable->appendChildTable();
}
OPENPAREN utterance[init] SEMICOLON exp[condition] SEMICOLON utterance[increment] CLOSEPAREN {
    AbstractValueASTNode *cond = NULL;
    if ($condition != NULL) {
        cond = (AbstractValueASTNode *)$condition;
        if (cond->getValueType() != typeBool)
            cond = new UnaryNode(UnarToBool, cond);
    }

    $$ = new ForNode($init, cond, $increment);
    ++g_LoopNestingCounter;
};

do_head : DO {
    ++g_LoopNestingCounter;
}

// Assignments ---------------------------------------------------------------

assignment : IDENTIFIER[name] ASSIGN exp[value] {
    AbstractSymbolTableRecord *var = currentTable->getVariableGlobal(*$name);
    if (var != NULL) {
        AbstractASTNode *node = numericAssign(var, (AbstractValueASTNode *)$value);
        if (node == NULL)
            YYERROR;
        $$ = node; }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$name));
        $$ = NULL;
        YYERROR;
    }
}
| IDENTIFIER[name] OPENBRACKET exp[index] CLOSEBRACKET ASSIGN exp[value] {
    AbstractSymbolTableRecord *var = currentTable->getVariableGlobal(*$name);
    if (var != NULL) {
        AbstractASTNode *node = arrayAssign(var, (AbstractValueASTNode *)$index, (AbstractValueASTNode *)$value);
        if (node == NULL)
            YYERROR;
        $$ = node; }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$name));
        $$ = NULL;
        YYERROR;
    }
};

// Expressions --------------------------------------------------------------

identifier_list : identifier_list[list] COMA IDENTIFIER[id] {
    if($list != NULL) {
        if(!currentTable->containsGlobal(*$id)) {
            AbstractSymbolTableRecord *var = currentTable->insertVariable(*$id, typeInt, enumCounter);
            if(var != NULL) {
                $$ = new ListNode(new AssignmentNode(var, new ValueNode(typeInt, enumCounter++)), $list);
            }
            else {
                $$ = NULL;
                YYERROR;
            }
        }
        else {
            parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(*$id));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }
}
| IDENTIFIER[id] {
    if(!currentTable->containsGlobal(*$id)) {
        AbstractSymbolTableRecord *var = currentTable->insertVariable(*$id, typeInt, enumCounter);
        if(var != NULL) {
            $$ = new AssignmentNode(var, new ValueNode(typeInt, enumCounter++));
        }
        else {
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(*$id));
        $$ = NULL;
        YYERROR;
    }
};


exp_list : exp COMA exp_list {
    $$ = new ListNode($1, $3);
}
| exp {
    $$ = $1;
};

exp : exp[left] RELOP exp[right] {
    // Relation exp

    AbstractValueASTNode *left = ((AbstractValueASTNode *)$left);
    AbstractValueASTNode *right = ((AbstractValueASTNode *)$right);

    if (isNumericType( left ) && isNumericType( right )) { // Type of variables is number.
        if (left->getValueType() != right->getValueType()) {// If exp have diffrence type.
            if (left->getValueType() == typeDouble || right->getValueType() == typeDouble) { // If left or right expression have type Double add to double
                if (left->getValueType() == typeDouble) {
                    $$ = new BinarNode($left, new UnaryNode(UnarToDouble, $right), $2, typeBool);
                }
                else {
                    $$ = new BinarNode(new UnaryNode(UnarToDouble, $left), $right, $2, typeBool);
                }
            }
            else { // if left or right expression have diffrent type of int converting to int
                if (left->getValueType() != typeInt)
                    left = new UnaryNode(UnarToInt, $left);
                if (right->getValueType() != typeInt)
                    right = new UnaryNode(UnarToInt, $right);

                $$ = new BinarNode($left, $right, $2, typeBool);
            }
        }
        else
            $$ = new BinarNode($left, $right, $2, typeBool);
    }
    else {
        if (left->getValueType() == right->getValueType()) {
            if (QString("==").contains(*$2))
                $$ = new BinarNode($left, $right, $2, typeBool);
            else {
                $$ = NULL;
                parsererror(errorList.at(ERROR_COMPARSION_NOT_APPLICABLE).arg($2).arg(left->getValueType()));
                YYERROR;
            }
        }
        else {
            $$ = NULL;
            parsererror(errorList.at(ERROR_COMPARSION_ON_DIFFERENCE_TYPES).arg($2).arg(left->getValueType()).arg(right->getValueType()));
            YYERROR;
        }
    }
}
| exp[left] AND exp[right] {
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "&", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] AND AND exp[right] {
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "&&", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] OR exp[right] {
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "|", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] OR OR exp[right] {
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "||", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| exp[left] XOR exp[right] {
    AbstractASTNode *p = binarBoolNode((AbstractValueASTNode *)$left, "^", (AbstractValueASTNode *)$right);
    if (p == NULL)
        YYERROR;
    $$ = p;
}
| NOT exp {
    AbstractValueASTNode *var = (AbstractValueASTNode *)$2;
    if (isNumericType( var )) {
        if (var->getValueType() != typeBool) {
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
| exp PLUS exp {
    // PLUS exp
    AbstractASTNode *p = binarMathNode((AbstractValueASTNode *)$1, $2, (AbstractValueASTNode *)$3);
    if (p == NULL)
        YYERROR;

    $$ = p;
}
| exp MINUS exp {
    // MINUS exp
    AbstractASTNode *p = binarMathNode((AbstractValueASTNode *)$1, $2, (AbstractValueASTNode *)$3);
    if (p == NULL)
        YYERROR;

    $$ = p;
}
| exp MULOP exp {
    // MULOP exp
    AbstractASTNode *p = binarMathNode((AbstractValueASTNode *)$1, $2, (AbstractValueASTNode *)$3);
    if (p == NULL)
        YYERROR;

    $$ = p;
}
| MINUS exp %prec UMINUS {
    $$ = new UnaryNode(UnarMinus, $2);
}
| OPENPAREN exp CLOSEPAREN {// ( exp )
    $$ = $2;
}
| REALCONST {
    $$ = new ValueNode(typeDouble, $1->toDouble());
}
| INTCONST {
    $$ = new ValueNode(typeInt, $1->toInt());
}
| CHARCONST {
    $$ = new ValueNode(typeChar, $1->toChar());
}
| STRING {
    $$ = new ValueNode(typeString, $1->toString());
}
| TRUE {
    $$ = new ValueNode(typeBool, 1);
}
| FALSE {
    $$ = new ValueNode(typeBool, 0);
}
| IDENTIFIER {
    AbstractSymbolTableRecord *var = getVariableByName(*$1);
    if (var != NULL) {
        $$ = new ReferenceNode(var);
    }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$1));
        $$ = NULL;
        YYERROR;
    }

}
| function_call {
    $$ = $1;
}
| IDENTIFIER[name] OPENBRACKET exp[index] CLOSEBRACKET {
    AbstractSymbolTableRecord *var = getVariableByName(*$name);
    if (var != NULL) {
        if (var->isArray()) {
            if (isNumericType( (AbstractValueASTNode *) $index) ) {
                $$ = new ArrayReferenceNode(var, ((AbstractValueASTNode *)$index));
            }
            else {
                parsererror(errorList.at(ERROR_CANNOT_USE_AS_INDEX).arg((typeName.at(((AbstractValueASTNode *)$index)->getValueType()))));
                $$ = NULL;
                YYERROR;
            }
        }
        else {
            parsererror(errorList.at(ERROR_VARIABLE_IS_NOT_ARRAY).arg((typeName.at(((AbstractValueASTNode *)$index)->getValueType()))));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$1));
        $$ = NULL;
        YYERROR;
    }
}
| scan ;

/*
 * Not in 2016.
 * | IDENTIFIER INCREMENT
{
    AbstractSymbolTableRecord *var = getVariableByName(*$1);

    if (isNumericType(var->valueType))
        $$ = createNodeAST(NT_UnaryOperation, $2, createReferenceNode(var), NULL);
    else
        parsererror(errorList.at(ERROR_INCREMENT_WRONG_TYPE).arg(typeName.at(var->valueType)).arg(*$1));

}
*/


%%
void yyerror(QString s) {
    if (printErrors)
        printf ("Line %d: %s.\n", lno, s.toStdString().c_str());
}

AbstractSymbolTableRecord *getVariableByName(QString name) {
    AbstractSymbolTableRecord *var = currentTable->getVariableGlobal(name);

    if (var == NULL)
    {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(name));
        //YYERROR;
    }
    return var;
}

bool isNumericType( ValueTypeEnum type )
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

bool isNumericType( AbstractValueASTNode *node )
{
    return isNumericType( ((AbstractValueASTNode *)node)->getValueType() );
}

AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if ( isNumericType(left) && isNumericType(right) ) {


        if (left->getValueType() != right->getValueType())
        {
            right = convert(right, left->getValueType());
        }
        if (right != NULL)
            return new BinarNode(left, right, operation);
        else
            return NULL;
    }
    else {
        parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE).arg(typeName.at(left->getValueType())).arg(typeName.at(right->getValueType())));
        return NULL;
    }
}

AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if ( isNumericType(left) && isNumericType(right) ) {
        if (operation == "&&" || operation == "||") {
            if (left->getValueType() != typeBool)
                left = new UnaryNode(UnarToBool, left);
            if (right->getValueType() != typeBool)
                right = new UnaryNode(UnarToBool, right);
        }

        return new BinarNode(left, right, operation, typeBool);
    }
    else {
        if ( !isNumericType(left) )
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(left->getValueType()))
                        .arg(typeName.at(typeBool)));
        if ( !isNumericType(right) )
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(right->getValueType()))
                        .arg(typeName.at(typeBool)));
        return NULL;
    }
}


AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value)
{
    if ( !currentTable->contains(name) ) {
        if (currentTable->getVariableGlobal(name) != NULL)
        {
            parsererror(errorList.at(WARNING_DOUBLE_DECLARED).arg(name));
        }


        AbstractSymbolTableRecord *var = currentTable->insertVariable(name, type, 0);
        if (var == NULL) {
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

AbstractASTNode *numericAssign(AbstractSymbolTableRecord *var, AbstractValueASTNode *value)
{

    if (var == NULL) {
        parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
        return NULL;
    }

    if (value != NULL) {
        if (!isNumericType(value))
        {
            parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE)
                        .arg(typeName.at(value->getValueType()))
                        .arg(typeName.at(var->getType())));
            return NULL;
        }
        else {
            if (value->getValueType() != var->getValueType()) {
                value = convert(value, var->getValueType());
            }

            if (value != NULL)
                return new AssignmentNode(var, value);
            else
                return NULL;
        }
    }
    else {
        return new AssignmentNode(var, new ValueNode(var->getValueType(), 0));
    }
}

AbstractASTNode *arrayAssign(AbstractSymbolTableRecord *var, AbstractValueASTNode *index, AbstractValueASTNode *value)
{

    if (var == NULL) {
        parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
        return NULL;
    }

    if (value != NULL) {
        if (!isNumericType(value))
        {
            parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE)
                        .arg(typeName.at(value->getValueType()))
                        .arg(typeName.at(var->getType())));
            return NULL;
        }
        else {
            if (value->getValueType() != var->getValueType()) {
                value = convert(value, var->getValueType());
            }

            if (value != NULL)
                return new ArrayAssignmentNode(var, index, value);
            else
                return NULL;
        }
    }
    else {
        return new ArrayAssignmentNode(var, index, new ValueNode(var->getValueType(), 0));
    }
}

AbstractValueASTNode *convert(AbstractValueASTNode *what, ValueTypeEnum to)
{
    ValueTypeEnum whatType = what->getValueType();
    if (isNumericType(whatType)) {
        if (isNumericType(to)) {
            switch(to){
            case typeBool:
                return new UnaryNode(UnarToBool, what);
            case typeChar:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToChar, what);
            case typeDouble:
                return new UnaryNode(UnarToDouble, what);;
            case typeFloat:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToFloat, what);
            case typeInt:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToInt, what);
            case typeShort:
                if (getSizeType(whatType) > getSizeType(to))
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
                .arg(typeName.at(what->getValueType()))
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

AbstractASTNode *validatedParams(FunctionTableRecord *function, AbstractASTNode *paramsNode)
{
    QList<ValueTypeEnum> callParamsTypes;
    AbstractASTNode *currentNode = paramsNode;
    AbstractASTNode *lastListNode = NULL;
    SymbolsTable::iterator pI = function->getParams()->begin();
    bool isValid = true;
    QString paramsString = "";

    if(currentNode != NULL) {
        // Bypass list
        while ( !currentNode->isValueNode() ) {
            AbstractValueASTNode *valueNode;

            if (currentNode->getType() == NT_List) {
                if (((ListNode *) currentNode)->getLeftNode()->isValueNode()) {
                    valueNode = ((AbstractValueASTNode *)((ListNode *) currentNode)->getLeftNode());
                }
                else {
                    parsererror(errorList.at(ERROR_PARAMS_FUNCTION).arg(function->getName()));
                    return NULL;
                }
            }
            else {
                parsererror(errorList.at(ERROR_PARAMS_FUNCTION).arg(function->getName()));
                return NULL;
            }

            if (pI != function->getParams()->end()) {
                if (valueNode->getValueType() !=  pI.value()->getValueType()) {
                    valueNode = convert(valueNode, pI.value()->getValueType());
                    if (valueNode != NULL) {
                        ((ListNode *) currentNode)->setLeftNode(valueNode);
                    }
                    else {
                        //error printing in convert function
                        return NULL;
                    }
                }}

            callParamsTypes << valueNode->getValueType();
            if(paramsString.length() != 0)
                paramsString.append(",");
            paramsString.append(typeName.at(valueNode->getValueType()));

            lastListNode = currentNode;
            currentNode = ((ListNode *) currentNode)->getRightNode();

            if (pI != function->getParams()->end()) {
                ++pI;
            }
        }
        if (pI != function->getParams()->end()) {
            if (((AbstractValueASTNode *) currentNode)->getValueType() !=  pI.value()->getValueType()) {
                currentNode =  convert(((AbstractValueASTNode *) currentNode), pI.value()->getValueType());
                if (currentNode != NULL) {
                    if( lastListNode != NULL ) {
                        ((ListNode *) lastListNode)->setRightNode(currentNode);
                    }
                    else {
                        // if one parametr and converting success returning there
                        return currentNode;
                    }
                }
                else {
                    //error printing in convert function
                    return NULL;
                }
            }
        }
        callParamsTypes << ((AbstractValueASTNode *) currentNode)->getValueType();

        if(paramsString.length() != 0)
            paramsString.append(",");
        paramsString.append(typeName.at(((AbstractValueASTNode *) currentNode)->getValueType()));

    }

    if (callParamsTypes.count() == function->getParams()->count()) {
        pI == function->getParams()->begin();
        for(int i = 0; i < callParamsTypes.count(); i++, pI++) {
            if (callParamsTypes.at(i) != pI.value()->getType())
            {
                parsererror(errorList.at(ERROR_CALL_UNDEFINED_FUNCTION_WITH_PARAMS).arg(function->getName()).arg(paramsString));
                return NULL;
            }
        }
        return paramsNode;
    }
    else {
        parsererror(errorList.at(ERROR_CALL_UNDEFINED_FUNCTION_WITH_PARAMS).arg(function->getName()).arg(paramsString));
    }

    return NULL;
}
