#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <QTextStream>
#include <QFile>

//#include "myast.h"
#include <map>
#include <tuple>
#include <vector>

#define DEBUG_MODE 1

#include "parser_yacc.h"

//extern int yyparse();
extern std::vector <std::tuple<std::string, std::string, int> > TokenTable;

FILE* outfile;
char g_outFileName[256];

typedef enum {
    SE_Tree,
    SE_Tokens,
    SE_Errors,
    SE_Out
} StreamsEnum;

QTextStream outStream;
QTextStream treeStream(stdout);
QTextStream tokenStream(stdout);
QTextStream errorStream(stdout);
QTextStream symbolsStream(stdout);
QTextStream cout(stdout);

QFile outFile;
QFile treeFile;
QFile tokensFile;
QFile errorsFile;
QFile symbolsFile;

bool printTree = false;
bool printTokens = false;
bool printWarnings = true;
bool printErrors = true;
bool printSymbols = false;

bool isFlag(QString arg);
bool changeStream(QTextStream *stream, QFile *file, QString fileName);
void printingTokens();

int main(int argc, char *argv[])
{
    //debugStream = new QTextStream;
    bool hasError = false;

    if (argc < 2)
    {
        printf("Too few paremeters. Need input file. Or use -h for help\n");
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if(isFlag(argv[i])) {
            switch (argv[i][1]) {
            case 'a':
                printTree = true;
                if (argc - 1 > i) {
                    if (!isFlag(argv[i+1])) {
                        i++;
                        if(!changeStream(&treeStream, &treeFile, argv[i])){
                            hasError = true;
                            cout << QString("Cannot open tree output file %1\n").arg(argv[i]);
                        }
                    }
                }
                break;
            case 'A':
                printTree = false;
            case 'e':
                printErrors = true;
                if (argc - 1 > i) {
                    if (!isFlag(argv[i+1])) {
                        i++;
                        if(!changeStream(&errorStream, &errorsFile, argv[i])){
                            hasError = true;
                            cout << QString("Cannot open error output file %1\n").arg(argv[i]);
                        }
                    }
                }
                break;
            case 'E':
                printErrors = false;
            case 'o': break;
            case 'O': break;
            case 's': break;
                printSymbols = true;
                if (argc - 1 > i) {
                    if (!isFlag(argv[i+1])) {
                        i++;
                        if(!changeStream(&symbolsStream, &symbolsFile, argv[i])){
                            hasError = true;
                            cout << QString("Cannot open error output file %1\n").arg(argv[i]);
                        }
                    }
                }
                break;
            case 'S': break;
                printSymbols = false;
            case 't':
                printTokens = true;
                if (argc - 1 > i) {
                    if (!isFlag(argv[i+1])) {
                        i++;
                        if(!changeStream(&tokenStream, &tokensFile, argv[i])){
                            hasError = true;
                            cout << QString("Cannot open token output file %1\n").arg(argv[i]);
                        }
                    }
                }
                break;
            case 'T': printTokens = false;
                break;
            case 'w':
                printWarnings = true;
            case 'W':
                printWarnings = false;
            case 'h':
                cout << "Using inputFileName [-A|-a [ASTFileName]] [-E|-e [errorFileName]] [-T|-t [tokenFileName]] [-W|-w]" << "\n"
                     << "-a\tprint abstract symbol tree." << "\n"
                     << "-A\tDon't print abstract symbol tree. Default not ptint" << "\n"
                     << "-e\tprint parser errors." << "\n"
                     << "-E\tDon't print parser errors. Default ptint to stdout" << "\n"
                     << "-t\tprint token table." << "\n"
                     << "-T\tDon't print token table. Default not ptint" << "\n"
                     << "-w\tshow warning messages." << "\n"
                     << "-W\tDon't show warning messages." << "\n";
            default:

                break;
            }
        }
        else {
            if(i == 1) {
                outFile.setFileName(argv[i]);
                if(outFile.open(QFile::ReadOnly)){
                    outStream.setDevice(&outFile);
                }
                else {
                    hasError = true;
                    cout << QString("Cannot open input file %1\n").arg(argv[i]);
                }
            }
            else {
                hasError = true;
                cout << "Error parametrs. For help use parameter -h\n";
            }
        }
    }

    cout.flush();

    if(!hasError) {
        if (NULL == freopen (argv[1], "r", stdin))
        {
            printf("Cannot open input file %s.\n", argv[1]);
            system("PAUSE");
            // not for *NIXes
            return EXIT_FAILURE;
        }

        int parsingResult = parserparse();

        if (DEBUG_MODE) {
            if(parsingResult == 0)
                cout << "Parser finish without errors" << "\n";
            else
                cout << "Parser has errors. Stop with result " << parsingResult << "\n";
        }
        treeStream << "\n";
        treeStream.flush();
        cout.flush();

        if(printTokens) {
            printingTokens();
        }
    }

}

bool changeStream(QTextStream *stream, QFile *file, QString fileName)
{
    file->setFileName(fileName);
    if(file->open(QFile::WriteOnly)){
        stream->setDevice(file);
        return true;
    }
    if(DEBUG_MODE)
        cout << "Error open file in function changeStream\n";
    return false;
}

bool isFlag(QString arg)
{
    return ((arg.at(0) == '-') && (arg.length() == 2));
}

void printingTokens()
{
    tokenStream << "Dump token table.\n";
    if (0 < TokenTable.size())
    {

        tokenStream.setPadChar(' ');
        tokenStream.setFieldAlignment(QTextStream::AlignLeft);
        tokenStream.setFieldWidth(25);

        tokenStream << "firstColName"
                    << "secondColName";
        tokenStream.setFieldWidth(0);
        tokenStream << "thirdColName\n";
        for(uint i = 0; i < TokenTable.size(); i++)
        {
            tokenStream.setFieldWidth(25);
            tokenStream << QString().fromStdString((std::string)(std::get<0>(TokenTable[i])))
                        << QString().fromStdString((std::string)(std::get<1>(TokenTable[i])));
            tokenStream.setFieldWidth(0);
            tokenStream << QString("%1\n").arg((int)(std::get<2>(TokenTable[i])));
        }
    }
    else {
        tokenStream << "No tokens." << "\n";
    }
    tokenStream.setFieldAlignment(QTextStream::AlignCenter);
    tokenStream << "\nDump finished.\n";

    tokenStream.flush();
}
