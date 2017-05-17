#ifndef PARSERSUPPORT
#define PARSERSUPPORT

#include "abstracttree/ast.h"
#include "symbolstable/abstractsymboltablerecord.h"
#include "symbolstable/symbolstable.h"

#define yyerror         parsererror


extern int lno;
extern SymbolsTable* currentTable;

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
        ERROR_CONTINUE_NOT_INSIDE_LOOP,
        ERROR_MEMORY_ALLOCATION,
        ERROR_UNRECOGNIZED_TOKEN,
        ERROR_UNFINISHED_STRING,
        ERROR_UNCLOSED_COMMENT,
        ERROR_COMPARSION_NOT_APPLICABLE,
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
        ERROR_PARAMS_FUNCTION,
        ERROR_IS_NOT_A_TYPE,
        ERROR_IS_NOT_A_STRUCT,
        ERROR_IS_NOT_A_MEMBER_STRUCT};

static QStringList errorList = QStringList() << "error: Variable %1 is already declared at this scope."
                                             << "warning: Variable %1 is already declared."
                                             << "error: Variable %1 was not declared."
                                             << "error: can not convert %1 to %2"
                                             << "error: 'break' not inside loop."
                                             << "error: 'continue' not inside loop."
                                             << "error: memory allocation or access error."
                                             << "error: %1 - unrecognized token."
                                             << "error: unfinished string."
                                             << "error: nonclosed comment"
                                             << "error: comparison oparation %1 not applicable for %2"
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
                                             << "error: params of call function %1 is not valid"
                                             << "error: %1 is not a type"
                                             << "error: variable %1 is not a structure"
                                             << "error: %1 is not a member struct %2";

bool contains(QString name);
AbstractSymbolTableRecord *getVariableByName(QString name);
void yyerror(QString s);
bool isNumericType( ValueTypeEnum type );
bool isNumericType( AbstractValueASTNode *node );
AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);
AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right);

AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value = NULL);
AbstractValueASTNode *convert(AbstractValueASTNode *what, ValueTypeEnum to);
AbstractASTNode *validatedParams(FunctionTableRecord *function, AbstractASTNode *paramsNode);

#endif // PARSERSUPPORT

