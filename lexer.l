%{
#include "lexer_constructions.h"
%}

%option nodefault noyywrap nounistd
%option yylineno
%option c++

stringch      [^"\n]
%%

    /* Skip all space symbols. */
[ \t\n\r]   { }


    /* Line comment. */
\/\/.*$     { }


    /* Noneclose multiline comment */
\/\*(.|[\t\n\r])* {
    cerr << "ERROR: " << YYText() << " - Unfinished comment. Line "
         << lineno() << ".\n";}


    /* Multiline comment. */
\/\*(.|[\t\n\r])*\*\/   { }


    /* Real constants. */
-?[0-9]+\.[0-9]+  {
    TokenTable.push_back(TokenTableRow(TokenClass::REALCONST, YYText(),
                                       lineno()));}


    /* Integer constanst. */
-?[0-9]+    {
    TokenTable.push_back(TokenTableRow(TokenClass::INTCONST, YYText(),
                                       lineno()));}


    /* Keywords. */
auto|break|case|char|const|continue { TokenTable.push_back(TokenTableRow(TokenClass::KEYWORD, YYText(), lineno())); }
default|do|double|else|extern       { TokenTable.push_back(TokenTableRow(TokenClass::KEYWORD, YYText(), lineno())); }
float|for|goto|if|int|print         { TokenTable.push_back(TokenTableRow(TokenClass::KEYWORD, YYText(), lineno())); }
return|scan|switch|void|while       { TokenTable.push_back(TokenTableRow(TokenClass::KEYWORD, YYText(), lineno())); }


    /* Punctuations. */
\;  { TokenTable.push_back(TokenTableRow(TokenClass::SEMICOLON, YYText(), lineno())); }
\:  { TokenTable.push_back(TokenTableRow(TokenClass::COLON, YYText(), lineno())); }
\,  { TokenTable.push_back(TokenTableRow(TokenClass::COMMA, YYText(), lineno())); }
\.  { TokenTable.push_back(TokenTableRow(TokenClass::DOT, YYText(), lineno())); }
\[  { TokenTable.push_back(TokenTableRow(TokenClass::OPENBRACKET, YYText(), lineno())); }
\]  { TokenTable.push_back(TokenTableRow(TokenClass::CLOSEBRACKET, YYText(), lineno())); }
\(  { TokenTable.push_back(TokenTableRow(TokenClass::OPENPAREN, YYText(), lineno())); }
\)  { TokenTable.push_back(TokenTableRow(TokenClass::CLOSEPAREN, YYText(), lineno())); }
\{  { TokenTable.push_back(TokenTableRow(TokenClass::OPENBRACE, YYText(), lineno())); }
\}  { TokenTable.push_back(TokenTableRow(TokenClass::CLOSEBRACE, YYText(), lineno())); }


    /* Operations. */
\=                          { TokenTable.push_back(TokenTableRow(TokenClass::ASSIGN, YYText(), lineno())); }
\+|\-                       { TokenTable.push_back(TokenTableRow(TokenClass::ADDOP, YYText(), lineno())); }
\*|\/|\%                    { TokenTable.push_back(TokenTableRow(TokenClass::MULOP, YYText(), lineno())); }
"&"|"|"|"^"|"!"             { TokenTable.push_back(TokenTableRow(TokenClass::BOOLOP, YYText(), lineno())); }
">="|"<="|"<"|">"|"=="|"!=" { TokenTable.push_back(TokenTableRow(TokenClass::RELOP, YYText(), lineno())); }


    /* Identifiers. */
[:alpha:]|[[:alnum:]_]* {
    TokenTable.push_back(TokenTableRow(TokenClass::IDENTIFIER, YYText(),
                                       lineno()));}


    /* Errors strings (without ") */
\"{stringch}*$  {
    cerr << "ERROR: " << YYText() << " - Unfinished string literal. Line "
         << lineno() << ".\n"; }


    /* Strings */
\"{stringch}*\" {
    TokenTable.push_back(TokenTableRow(TokenClass::STRING, YYText(),
                                       lineno())); }


    /* Other, not content in alphapetic. */
.   {
    cerr << "ERROR: " << YYText() << " - Unrecognized token. Line "
         << lineno() << ".\n"; }

%%
