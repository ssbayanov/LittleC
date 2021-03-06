%code requires { //code insert in top parser_yacc.h

#include <iostream>
#include <QString>
#include <QStringList>

#include "parsersupport.h"
#include <QDebug>
}

%{
#include "parser_yacc.h"
#include "parsersupport.h"

#define YYERROR_VERBOSE         1

extern int lno;
extern int parserlex();

int loopSwitchCounter = 0;
int ifCounter = 0;
int enumCounter = 0;

SymbolsTable* topLevelVariableTable = new SymbolsTable(NULL);
SymbolsTable* currentTable = topLevelVariableTable;

%}

%union
{
    AbstractASTNode *astNode; // node of abstract three
    QString *variableName; // name variable
    char opName[3]; // operation text
    QVariant *value; // value of constant
    ValueTypeEnum type; // type of data
}

// Declare tokens.
%token          EOFILE 0    "end of file"
%token <value>  REALCONST   "floating point double precision"
%token <value>  INTCONST    "integer constant"
%token <value>  CHARCONST   "char constant"
%token <value>  STRING      "string"
%token <variableName> IDENTIFIER "indentifer"
%token <opName> RELOP       "relation operator"
%token <opName> PLUS        "+"
%token <opName> MINUS       "-"
%token <opName> MULOP       "mulop"
%token          INCREMENT   "increment"
%token          DECREMENT   "decrement"
%token          AND         "and"
%token          OR          "or"
%token          NOT         "not"
%token          XOR         "xor"
%token          OPENBRACE   "{"
%token          CLOSEBRACE  "}"
%token          OPENPAREN   "("
%token          CLOSEPAREN  ")"
%token          OPENBRACKET "["
%token          CLOSEBRACKET"]"
%token          ASSIGN      "="
%token          SEMICOLON   ";"
%token          COLON       ":"
%token          COMA        ","
%token          DOT         "."
// Control constructions.
%token          IF         "if"
%token          ELSE       "else"
%token          FOR        "for"
%token          WHILE      "while"
%token          DO         "do"
%token          SWITCH     "switch"
%token          CASE       "case"
%token          DEFAULT    "default"
%token          CONTINUE   "continue"
%token          BREAK      "break"
%token          GOTO       "goto"
// Types of data.
%token          INT        "int"
%token          DOUBLE     "double"
%token          FLOAT      "float"
%token          BOOL       "bool"
%token          CHAR       "char"
%token          SHORT      "short"
%token          ENUM       "enum"
%token          VOID       "void"
%token          STRUCT     "struct"
// Functions.
%token          SCAN       "scan"
%token          PRINT      "print"
//
%token          TRUE       "true"
%token          FALSE      "false"

%token          RETURN     "return"

/*%token IFX */

%type <astNode>  program statement utterance statement_list statement_tail
// Data types
declaration declaration_list_element declaration_list enum_decloration_statement
parameter parameter_list
// Operations
assignment compound_statement exp exp_list identifier_list
reference variable_reference increment
// Array
array_declaration array_declaration_statement array_reference array_assignment_statement
// Structure
struct_decloration_statement struct_variable_declaration_statement
srtuct_variable_reference srtuct_variable_assignment_statement
// Control statements
condition_statement
switch_statement case_statement case_list case_tail
loop_statement while_head break_statement continue_statement for_head
goto_statement label_statement
// Functions
function_call function_description_statement function_return_statement
// IO Functions
print scan

%type <type> data_types

/* Priority */
%nonassoc IF
%nonassoc ELSE
%right ASSIGN
%left AND OR XOR
%left RELOP
%left PLUS MINUS
%left MULOP
%left ADDUOP
%right UMINUS
//%right data_types function_types
%left INCREMENT
%left DECREMENT
%left OPENPAREN CLOSEPAREN OPENBRACKET CLOSEBRACKET

%start program

%destructor {
    delete $$;
} IDENTIFIER
%%

program : statement_list
{
    if(printTree) {
        $1->printNode(0);
    }

    $1->printTripleCode();
    $1->~AbstractASTNode();

    topLevelVariableTable->~SymbolsTable();
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
| function_call
| increment;

statement : condition_statement
| compound_statement
| break_statement
| continue_statement
| loop_statement
| switch_statement
| goto_statement
| label_statement
| array_declaration_statement
| array_assignment_statement
| function_description_statement
| function_return_statement
| enum_decloration_statement
| struct_decloration_statement
| struct_variable_declaration_statement
| srtuct_variable_assignment_statement
| utterance SEMICOLON {
    $$ = $1;
}
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
parameter_list[params] CLOSEPAREN compound_statement[body] {
    currentTable->setHidden();
    SymbolsTable *paramsTable = currentTable;
    currentTable = currentTable->getParent();

    if(!contains(*$name)) {
        AbstractSymbolTableRecord *function = currentTable->insertFunction(*$name, $type, paramsTable);
        $$ = new FunctionDeclareNode(function, $params, $body);
    }
    else {
        $$ = NULL;
        YYERROR;
    }
};

function_call : IDENTIFIER[id] OPENPAREN exp_list[params] CLOSEPAREN
{
    AbstractSymbolTableRecord *function = getVariableByName(*$id);
    if (function != NULL) {
        if (function->isFunction()) {
            AbstractASTNode *validParams = validatedParams(((FunctionTableRecord *) function), $params);
            if (validParams != NULL) {
                if(validParams->getType() == NT_List) {
                    ((ListNode *) validParams)->setListType(LT_CallParamList);
                }
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

array_declaration : data_types[type] IDENTIFIER[name] OPENBRACKET exp[size] CLOSEBRACKET {
    if(!contains(*$name)) {
        AbstractSymbolTableRecord *array = currentTable->insertArray(*$name, $type);
        if (array != NULL) {
            $$ = new ArrayDeclareNode(array, NULL, $size);
        }
        else {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }

}
| data_types[type] IDENTIFIER[name] OPENBRACKET CLOSEBRACKET ASSIGN OPENBRACE exp_list[values] CLOSEBRACE {
    if(!contains(*$name)) {
        AbstractSymbolTableRecord *array = currentTable->insertArray(*$name, $type);
        if (array != NULL) {
            $$ = new ArrayDeclareNode(array, $values, NULL);
        }
        else {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }
}
| data_types[type] IDENTIFIER[name] OPENBRACKET CLOSEBRACKET ASSIGN STRING[value] {
    if(!contains(*$name)) {
        if ($type == typeChar) {
            AbstractSymbolTableRecord *array = currentTable->insertArray(*$name, $type);
            if (array != NULL) {
                $$ = new ArrayDeclareNode(array, new ValueNode(typeString, *$value), NULL);
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
    }
    else {
        $$ = NULL;
        YYERROR;
    }
};

array_reference : IDENTIFIER[name] OPENBRACKET exp[index] CLOSEBRACKET {
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

array_assignment_statement : IDENTIFIER[id] OPENBRACKET exp[index] CLOSEBRACKET ASSIGN exp[value] SEMICOLON{
    AbstractSymbolTableRecord *var = currentTable->getVariableGlobal(*$id);
    if (var != NULL) {
        $$ = new ArrayAssignmentNode(var, $index, $value);
    }
    else {
        $$ = NULL;
        YYERROR;
    }
}

// Struct -------------------------------------------------------------------------

struct_decloration_statement : STRUCT IDENTIFIER[name] {
    currentTable = currentTable->appendChildTable();
} OPENBRACE declaration_list[values] CLOSEBRACE SEMICOLON{
    SymbolsTable *variables = currentTable;
    currentTable->setHidden();
    currentTable = currentTable->getParent();
    if(!contains(*$name)) {
        AbstractSymbolTableRecord *structure = currentTable->insertStructType(*$name, variables);
        if (structure != NULL) {
            $$ = new StructDeclareNode(structure, $values);
        }
        else {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }
};

struct_variable_declaration_statement : IDENTIFIER[struct_name] IDENTIFIER[name] SEMICOLON{
    AbstractSymbolTableRecord *structType = currentTable->getVariableGlobal(*$struct_name);
    if (structType != NULL) {
        if (structType->isStructType()) {
            if (!contains(*$name)) {
                AbstractSymbolTableRecord *structVariable = currentTable->insertStruct(*$name, (StructTypeTableRecord *) structType);
                if (structVariable != NULL) {
                    $$ = new StructVariableDeclareNode(structType, structVariable);
                }
                else {
                    parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
                    $$ = NULL;
                    YYERROR;
                }
            }
            else {
                $$ = NULL;
                YYERROR;
            }
        }
        else {
            parsererror(errorList.at(ERROR_IS_NOT_A_TYPE).arg(structType->getName()));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        parsererror(errorList.at(ERROR_IS_NOT_A_TYPE).arg(structType->getName()));
        $$ = NULL;
        YYERROR;
    }

};

srtuct_variable_reference : IDENTIFIER[struct_name] DOT IDENTIFIER[name] {
    AbstractSymbolTableRecord *structVariable = currentTable->getVariableGlobal(*$struct_name);
    if (structVariable != NULL) {
        if (structVariable->isStruct()) {
            AbstractSymbolTableRecord *member = ((StructTableRecord *) structVariable)->getStructType()->getVariables()->getVariable(*$name);
            if (member != NULL) {
                $$ = new StructReferenceNode(structVariable, member);
            }
            else {
                parsererror(errorList.at(ERROR_IS_NOT_A_MEMBER_STRUCT).arg(*$name).arg(*$struct_name));
                $$ = NULL;
                YYERROR;
            }
        }
        else {
            parsererror(errorList.at(ERROR_IS_NOT_A_STRUCT).arg(*$struct_name));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$struct_name));
        $$ = NULL;
        YYERROR;
    }
}

srtuct_variable_assignment_statement : IDENTIFIER[struct_name] DOT IDENTIFIER[name] ASSIGN exp[value] SEMICOLON{
   AbstractSymbolTableRecord *structVariable = currentTable->getVariableGlobal(*$struct_name);
   if (structVariable != NULL) {
       if (structVariable->isStruct()) {
           AbstractSymbolTableRecord *member = ((StructTableRecord *) structVariable)->getStructType()->getVariables()->getVariable(*$name);
           if (member != NULL) {
               AbstractValueASTNode *value = ((AbstractValueASTNode *) $value);
               if(value->getValueType() != member->getValueType()) {
                   value = convert(value, member->getValueType());
               }

               $$ = new StructVariableAssignment(structVariable, member, value);
           }
           else {
               parsererror(errorList.at(ERROR_IS_NOT_A_MEMBER_STRUCT).arg(*$name).arg(*$struct_name));
               $$ = NULL;
               YYERROR;
           }
       }
       else {
           parsererror(errorList.at(ERROR_IS_NOT_A_STRUCT).arg(*$struct_name));
           $$ = NULL;
           YYERROR;
       }
   }
   else {
       parsererror(errorList.at(ERROR_NOT_DECLARED).arg(*$struct_name));
       $$ = NULL;
       YYERROR;
   }
}


// Numeric declaration ------------------------------------------------------------
enum_decloration_statement : ENUM { enumCounter = 0; } OPENBRACE identifier_list[values] CLOSEBRACE SEMICOLON{
    $$ = $values;
}


declaration_list : declaration SEMICOLON declaration_list_element[tail] {
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
| declaration_list
{
    $$ = $1;
};

parameter_list : parameter[param] COMA parameter_list[list] {
    $$ = new ListNode($param, $list, LT_DeclareParamList);
}
| parameter[param]
| %empty {
    $$ = NULL;
};

parameter : data_types[type] IDENTIFIER{
    if (isNumericType( $type )) {
        AbstractASTNode *var = numericDeclaration($type, *$2);
        if (var == NULL)
            YYERROR;
        $$ = var;
    }
    else {
        $$ = NULL;
    }
}
/*| data_types[type] IDENTIFIER[name] OPENBRACKET CLOSEBRACKET {
    if (isNumericType( $type )) {
        AbstractASTNode *var = NULL;//arrayDeclaration($1, *$2);
        if (var == NULL)
            YYERROR;
        $$ = var;
    }
    else {
        $$ = NULL;
    }
}*/
;


declaration : parameter
| data_types[type] IDENTIFIER ASSIGN exp
{
    AbstractASTNode *var = numericDeclaration($type, *$2, (AbstractValueASTNode *)$4);
    if (var == NULL)
        YYERROR;
    $$ = var;
}
| array_declaration;

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
    if (loopSwitchCounter == 0) {
        parsererror(errorList.at(ERROR_BREAK_NOT_INSIDE_LOOP));
        $$ = NULL;
    }
    else {
        $$ = new GoToNode("$end$");
    }
}

continue_statement : CONTINUE SEMICOLON {
    if (loopSwitchCounter == 0) {
        parsererror(errorList.at(ERROR_CONTINUE_NOT_INSIDE_LOOP));
        $$ = NULL;
    }
    else {
        $$ = new GoToNode("$continue$");
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

switch_statement : SWITCH {
    loopSwitchCounter++;

} OPENPAREN exp CLOSEPAREN OPENBRACE case_list CLOSEBRACE {

    $$ = new SwitchNode($exp, $case_list);
    loopSwitchCounter--;
};

//Print and scan-------------------------------------------------------------------
print : PRINT OPENPAREN exp CLOSEPAREN {
    AbstractValueASTNode *value = ((AbstractValueASTNode *) $3);
    if(value->getValueType() == typeFloat)
        value = convert(value, typeDouble);
    $$ = new PrintNode(value);
};

scan: SCAN OPENPAREN STRING[typeString] CLOSEPAREN {
    ValueTypeEnum scanType = typeVoid;
    if($typeString->toString().at(0) == '%') {
        switch($typeString->toString().at(1).toLatin1()) {
        case 'd':
        case 'i': scanType = typeInt; break;
        case 'e':
        case 'f': scanType = typeFloat; break;
        case 'l': scanType = typeDouble; break;
        case 's': scanType = typeString; break;
        case 'c': scanType = typeChar; break;
        default: parsererror(errorList.at(ERROR_SCAN_STRING));
            $$ = NULL;
            YYERROR;
        }
        if(scanType != typeVoid) {
            $$ = new ScanNode(scanType);
        }
        else {
            parsererror(errorList.at(ERROR_SCAN_STRING));
            $$ = NULL;
            YYERROR;
        }
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
    loopSwitchCounter--;
    ((WhileNode *)$1)->setBody($2);
    $$ = $1;

}
| for_head statement {
    loopSwitchCounter--;
    ((ForNode *)$1)->setBody($2);
    $$ = $1;


    currentTable->setHidden();
    currentTable = currentTable->getParent();
}
| do_head statement while_head SEMICOLON{
    loopSwitchCounter--;
    ((WhileNode *)$while_head)->setBody($2);
    ((WhileNode *)$while_head)->setIsDoWhile(true);
    $$ = $3;

}
;

while_head : WHILE OPENPAREN exp[condition] CLOSEPAREN {
    loopSwitchCounter++;
    AbstractValueASTNode *cond = NULL;
    if ($condition != NULL) {
        cond = (AbstractValueASTNode *)$condition;
        if (cond->getValueType() != typeBool)
            cond = new UnaryNode(UnarToBool, cond);
    }

    $$ = new WhileNode(cond);
};

for_head : FOR {
    currentTable = currentTable->appendChildTable();
    loopSwitchCounter++;
}
OPENPAREN utterance[init] SEMICOLON exp[condition] SEMICOLON utterance[increment] CLOSEPAREN {
    AbstractValueASTNode *cond = NULL;
    if ($condition != NULL) {
        cond = (AbstractValueASTNode *)$condition;
        if (cond->getValueType() != typeBool)
            cond = new UnaryNode(UnarToBool, cond);
    }

    $$ = new ForNode($init, cond, $increment);
};

do_head : DO {
    loopSwitchCounter++;
}

// Assignments ---------------------------------------------------------------

assignment : IDENTIFIER[id] ASSIGN exp[value] {
    AbstractSymbolTableRecord *var = currentTable->getVariableGlobal(*$id);
    if (var != NULL) {
        AbstractValueASTNode *value = ((AbstractValueASTNode *) $value);
        if(value->getValueType() == var->getValueType())
            $$ = new AssignmentNode(var, $value);
        else{
            value = convert(value, var->getValueType());
            if(value != NULL) {
                $$ = new AssignmentNode(var, value);
            }
            else {
                $$ = NULL;
                YYERROR;
            }
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }
};

// Expressions --------------------------------------------------------------

increment : IDENTIFIER[name] INCREMENT
{
    AbstractSymbolTableRecord *var = getVariableByName(*$name);
    if(var != NULL) {
        if (isInt(var->getValueType())){
            $$ = new UnaryNode(UnarIncrement, var);
        }
        else {
            parsererror(errorList.at(ERROR_INCREMENT_WRONG_TYPE).arg(typeName.at(var->getValueType())).arg(*$name));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }

}

| INCREMENT IDENTIFIER[name]
{
    AbstractSymbolTableRecord *var = getVariableByName(*$name);
    if(var != NULL) {
        if (isInt(var->getValueType())){
            $$ = new UnaryNode(UnarPreincrement, var);
        }
        else {
            parsererror(errorList.at(ERROR_INCREMENT_WRONG_TYPE).arg(typeName.at(var->getValueType())).arg(*$name));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }
}
| IDENTIFIER[name] DECREMENT
{
    AbstractSymbolTableRecord *var = getVariableByName(*$name);
    if(var != NULL) {
        if (isInt(var->getValueType())){
            $$ = new UnaryNode(UnarDecrement, var);
        }
        else {
            parsererror(errorList.at(ERROR_INCREMENT_WRONG_TYPE).arg(typeName.at(var->getValueType())).arg(*$name));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }

}

| DECREMENT IDENTIFIER[name]
{
    AbstractSymbolTableRecord *var = getVariableByName(*$name);
    if(var != NULL) {
        if (isInt(var->getValueType())){
            $$ = new UnaryNode(UnarPredecrement, var);
        }
        else {
            parsererror(errorList.at(ERROR_INCREMENT_WRONG_TYPE).arg(typeName.at(var->getValueType())).arg(*$name));
            $$ = NULL;
            YYERROR;
        }
    }
    else {
        $$ = NULL;
        YYERROR;
    }
};

identifier_list : identifier_list[list] COMA IDENTIFIER[id] {
    if($list != NULL) {
        if(!currentTable->contains(*$id)) {
            AbstractSymbolTableRecord *var = currentTable->insertVariable(*$id, typeInt, enumCounter);
            if(var != NULL) {
                $$ = new ListNode(new BinarNode(new ReferenceNode(var), new ValueNode(typeInt, enumCounter++), "="), $list);
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
    if(!currentTable->contains(*$id)) {
        AbstractSymbolTableRecord *var = currentTable->insertVariable(*$id, typeInt, enumCounter);
        if(var != NULL) {
            $$ = new BinarNode(new ReferenceNode(var), new ValueNode(typeInt, enumCounter++), "=");
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

reference : array_reference
| variable_reference
| srtuct_variable_reference;

variable_reference :  IDENTIFIER {
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
        parsererror(errorList.at(ERROR_COMPARSION_NOT_APPLICABLE).arg($2).arg(typeName.at(left->getValueType())));
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
| function_call
| increment
| scan
| reference;
%%
