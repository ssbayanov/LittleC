#ifndef LEXER_CONSTRUCTIONS_H
#define LEXER_CONSTRUCTIONS_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <tuple>
#include <vector>
#include <string>

using namespace std;

enum class TokenClass
{
  INTCONST,
  REALCONST,
  KEYWORD,
  SEMICOLON,
  COLON,
  COMMA,
  DOT,
  OPENPAREN,
  CLOSEPAREN,
  OPENBRACE,
  CLOSEBRACE,
  OPENBRACKET,
  CLOSEBRACKET,
  ASSIGN,
  ADDOP,
  MULOP,
  RELOP,
  BOOLOP,
  IDENTIFIER,
  STRING
};

static std::map <TokenClass, string> TokenClassNames =
{
  { TokenClass::INTCONST,   "INTCONST" },
  { TokenClass::REALCONST,  "REALCONST" },
  { TokenClass::KEYWORD,    "KEYWORD" },
  { TokenClass::SEMICOLON,  "SEMICOLON" },
  { TokenClass::COLON,      "COLON" },
  { TokenClass::COMMA,      "COMMA" },
  { TokenClass::DOT,        "DOT" },
  { TokenClass::OPENPAREN,  "OPENPAREN" },
  { TokenClass::CLOSEPAREN, "CLOSEPAREN" },
  { TokenClass::OPENBRACE,  "OPENBRACE" },
  { TokenClass::CLOSEBRACE, "CLOSEBRACE" },
  { TokenClass::OPENBRACKET,"OPENBRACKET" },
  { TokenClass::CLOSEBRACKET,"CLOSEBRACKET" },
  { TokenClass::ASSIGN,     "ASSIGN" },
  { TokenClass::ADDOP,      "ADDOP" },
  { TokenClass::MULOP,      "MULDOP" },
  { TokenClass::RELOP,      "RELDOP" },
  { TokenClass::BOOLOP,     "BOOLOP" },
  { TokenClass::IDENTIFIER, "IDENTIFIER" },
  { TokenClass::STRING,     "STRING" }
};

typedef std::tuple<TokenClass, string, int> TokenTableRow;

std::vector <TokenTableRow> TokenTable;


static void dumpTokenTable(string firstColName, string secondColName, string thirdColName)
{
    cout << left;
    cout.width(20);  cout << firstColName;
    cout.width(20);  cout << secondColName;
    cout.width(20);  cout << thirdColName << "\n";
    for(TokenTableRow ttr : TokenTable)
    {
      cout.width(20);
      cout << TokenClassNames[std::get<0>(ttr)];
      cout.width(20);
      cout << get<1>(ttr) << get<2>(ttr) << "\n";
    }
}

#endif // LEXER_CONSTRUCTIONS_H
