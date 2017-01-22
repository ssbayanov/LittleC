%{
/*
 * LittleC lexer analisator
 *
 * Development by Stanislav Bayanov ssbayanov@gmail_dot_com
 * and Sokolova Mary
 * Coursework 10 semester
 * SFU IKIT
 * Lecturer Alexandr Sergeevich Kuznecov
 */

#if defined _WIN32
#include <io.h> // Для isatty
#elif defined _WIN64
#include <io.h> // Для isatty
#endif

#ifdef MSVC
#define isatty _isatty // В VC isatty назван _isatty
#endif

#include <ctype.h>
#include <iostream>
#include <fstream>
//#include <cstdlib>
#include <stdlib.h>
#include <map>
#include <tuple>
#include <vector>
#include <string.h>

#include "parser_yacc.h"


bool multilineComment();

#define lexerlex parserlex

typedef std::tuple<std::string, std::string, int> TokenTableRow;
std::vector <std::tuple<std::string, std::string, int> > TokenTable;

#define yyterminate() return EOFILE

int lno = 1;
%}

%option nodefault noyywrap

stringch      [^"\n]

%%

    /* Counting lines. */
\n {lno++;}


    /* Skip all space symbols. */
[ \t\n\r]   { }


    /* Line comment. */
\/\/.*$     { }


    /* Multiline comment. */
\/\* { bool isClosed = multilineComment();
        if(!isClosed)
            parsererror(errorList.at(ERROR_UNCLOSED_COMMENT));
    }


    /* Binar operations. */
\=                          {
                                //TokenTable.push_back(TokenTableRow("ASSIGN", yytext, lno));
                                return ASSIGN;
                            }
    /*
     * Not in 2016.
     * \+{2}|\-{2}                     {
                                TokenTable.push_back(TokenTableRow("ADDUOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return INCREMENT;
                            }*/

\+                          {
                                //TokenTable.push_back(TokenTableRow("PLUS", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return PLUS;
                            }

\-                          {
                                //TokenTable.push_back(TokenTableRow("MINUS", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return MINUS;
                            }

    /*  */
\*|\/|\%                    {
                                TokenTable.push_back(TokenTableRow("MULOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return MULOP;
                            }

    /* Boolean operations */
"&"                         {
                                TokenTable.push_back(TokenTableRow("BOOLOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return AND;
                            }
"|"                         {
                                TokenTable.push_back(TokenTableRow("BOOLOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return OR;
                            }
"^"                         {
                                TokenTable.push_back(TokenTableRow("BOOLOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return XOR;
                            }
"!"                         {
                                TokenTable.push_back(TokenTableRow("BOOLOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return NOT;
                            }

    /* Comparison and relation operator tokens */
">="|"<="|"<"|">"|"=="|"!=" {
                                TokenTable.push_back(TokenTableRow("RELOP", yytext, lno));
                                strcpy(parserlval.opName, yytext);
                                return RELOP;
                            }

    /* Punctuation */
\;                          {
                                TokenTable.push_back(TokenTableRow("SEMICOLON", yytext, lno));
                                return SEMICOLON;
                            }

\:                          {
                                TokenTable.push_back(TokenTableRow("COLON", yytext, lno));
                                return COLON;
                            }

\,                          {
                                TokenTable.push_back(TokenTableRow("COMA", yytext, lno));
                                return COMA;
                            }

\.                          {
                                TokenTable.push_back(TokenTableRow("DOT", yytext, lno));
                                return DOT;
                            }

\[                          {
                                TokenTable.push_back(TokenTableRow("OPENBRACKET", yytext, lno));
                                return OPENBRACKET;
                            }

\]                          {
                                TokenTable.push_back(TokenTableRow("CLOSEBRACKET", yytext, lno));
                                return CLOSEBRACKET;
                            }

\(                          {
                                TokenTable.push_back(TokenTableRow("OPENPAREN", yytext, lno));
                                return OPENPAREN;
                            }

\)                          {
                                TokenTable.push_back(TokenTableRow("CLOSEPAREN", yytext, lno));
                                return CLOSEPAREN;
                            }

\{                          {
                                TokenTable.push_back(TokenTableRow("OPENBRACE", yytext, lno));
                                return OPENBRACE;
                            }

\}                          {
                                TokenTable.push_back(TokenTableRow("CLOSEBRACE", yytext, lno));
                                return CLOSEBRACE;
                            }

    /* Control commands. */
if       { TokenTable.push_back(TokenTableRow("IF", yytext, lno)); return IF; }
else     { TokenTable.push_back(TokenTableRow("ELSE", yytext, lno)); return ELSE; }
for      { TokenTable.push_back(TokenTableRow("FOR", yytext, lno)); return FOR; }
while    { TokenTable.push_back(TokenTableRow("WHILE", yytext, lno)); return WHILE; }
do       { TokenTable.push_back(TokenTableRow("DO", yytext, lno)); return DO; }
switch   { TokenTable.push_back(TokenTableRow("SWITCH", yytext, lno)); return SWITCH; }
case     { TokenTable.push_back(TokenTableRow("CASE", yytext, lno)); return CASE; }
default  { TokenTable.push_back(TokenTableRow("DEFAULT", yytext, lno)); return DEFAULT; }
continue { TokenTable.push_back(TokenTableRow("CONTINUE", yytext, lno)); return CONTINUE; }
break    { TokenTable.push_back(TokenTableRow("BREAK", yytext, lno)); return BREAK; }
goto     { TokenTable.push_back(TokenTableRow("GOTO", yytext, lno)); return GOTO; }

return   { TokenTable.push_back(TokenTableRow("RETURN", yytext, lno)); return RETURN; }

    /* Data types. */
short    { TokenTable.push_back(TokenTableRow("SHORT", yytext, lno)); return SHORT; }
int      { TokenTable.push_back(TokenTableRow("INT", yytext, lno)); return INT; }
char     { TokenTable.push_back(TokenTableRow("CHAR", yytext, lno)); return CHAR; }
bool     { TokenTable.push_back(TokenTableRow("BOOL", yytext, lno)); return BOOL; }
float    { TokenTable.push_back(TokenTableRow("FLOAT", yytext, lno)); return FLOAT; }
double   { TokenTable.push_back(TokenTableRow("DOUBLE", yytext, lno)); return DOUBLE; }
void     { TokenTable.push_back(TokenTableRow("VOID", yytext, lno)); return VOID; }

    /* User data types. */
enum     { TokenTable.push_back(TokenTableRow("ENUM", yytext, lno)); return ENUM; }
struct   { TokenTable.push_back(TokenTableRow("STRUCT", yytext, lno)); return STRUCT; }

    /* Predestined values. */
true     { TokenTable.push_back(TokenTableRow("TRUE", yytext, lno)); return TRUE; }
false    { TokenTable.push_back(TokenTableRow("FALSE", yytext, lno)); return FALSE; }

    /* Integrated functions. */
print    { TokenTable.push_back(TokenTableRow("PRINT", yytext, lno)); return PRINT; }
scan     { TokenTable.push_back(TokenTableRow("SCAN", yytext, lno)); return SCAN; }

    /* Real constants. */
[0-9]+\.[0-9]+ {TokenTable.push_back(TokenTableRow("REALCONST", yytext, lno)); parserlval.value = new QVariant(yytext);
            return REALCONST;}

    /* Integer constanst. */
[0-9]+ {TokenTable.push_back(TokenTableRow("INTCONST", yytext, lno)); parserlval.value = new QVariant(yytext);
            return INTCONST;}

    /* Identifiers. */
[:alpha:]|[[:alnum:]_]* {TokenTable.push_back(TokenTableRow("IDENTIFIER", yytext, lno)); parserlval.variableName = new QString(yytext);
return IDENTIFIER;}

    /* Errors strings (without ") */
 \"{stringch}*$  {
     parsererror(errorList.at(ERROR_UNFINISHED_STRING));}

    /* Char */
\'{stringch}{1}\' {
    TokenTable.push_back(TokenTableRow("CHARCONST", yytext, lno));
    QString c = QString(yytext);
    parserlval.value = new QVariant(c.at(1));
    return CHARCONST;}

    /* String */
\"{stringch}*\" {
     TokenTable.push_back(TokenTableRow("STRING", yytext, lno));
     QString str = QString(yytext);
     parserlval.value = new QVariant(str.mid(1,str.length() - 2));
     return STRING;
     }

    /* Undefined token. */
.       {
        parsererror(errorList.at(ERROR_UNRECOGNIZED_TOKEN).arg(yytext));
        }


<<EOF>> {
        return EOFILE;
        }


%%
bool multilineComment(){
    char    c1 = ' ', c2 = ' ';
    while(c1 != '*' || c2 != '/')
    {
        c1 = c2;
        c2 = yyinput();
        if(c2 == EOF)
            return false;

        if(c2 == '\n')
            lno++;
    }

    return true;
}
