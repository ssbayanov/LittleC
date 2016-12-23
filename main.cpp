//#include <QtCore/QCoreApplication>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "myast.h"
#include <map>
#include <tuple>
#include <vector>

//extern int yylex();
extern int yyparse();
//extern FILE *yyin;
FILE* outfile;
char g_outFileName[256];
#include "parser_yacc.h"

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("Too few paremeters.\n");
        system("PAUSE"); // not for *NIXes
        return EXIT_FAILURE;
    }
    if (NULL == freopen (argv[1], "r", stdin))
    {
        printf("Cannot open input file %s.\n", argv[1]);
        system("PAUSE");
        // not for *NIXes
        return EXIT_FAILURE;
    }
    outfile = fopen(argv[2], "w");
    if (NULL == outfile)
    {
        printf("Cannot open output file %s.\n", argv[2]);
        system("PAUSE");
        // not for *NIXes
        return EXIT_FAILURE;
    }
    strcpy(g_outFileName, argv[2]);
    parserparse();

    std::cout << "Dump token table.\n\n";
    if (0 < TokenTable.size())
    {
        std::cout << std::left;
        std::cout.width(20);  std::cout << "firstColName";
        std::cout.width(20);  std::cout << "secondColName";
        std::cout.width(20);  std::cout << "thirdColName" << "\n";
        for(int i = 0; i < TokenTable.size(); i++)
        {
            std::cout.width(20);
            std::cout << std::get<0>(TokenTable[i]);
            std::cout.width(20);
            std::cout << std::get<1>((TokenTable[i])) << std::get<2>((TokenTable[i])) << "\n";
        }

        //DumpTokenTable("Token class", "Token value", "Line number");
    }
    else
    {
        std::cout << "No tokens." << "\n";
    }
    std::cout << "\nDump finished.\n\n";


    fclose(outfile);

}
