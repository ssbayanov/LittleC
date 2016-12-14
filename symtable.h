#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <string>
#include <vector>
#include "subexpression.h"

/* Symbols table record definition */
typedef struct
{
  std::string* name;      /* Multiple character variable name */
  SubexpressionValueTypeEnum valueType; /* Type of a variable or expression */
  double value;   /* Currently not used, reserved to the future */
} TSymbolTableRecord;

typedef struct SymbolTable
{
  struct SymbolTable* parentTable;
  bool isHidden;
  std::vector <TSymbolTableRecord> data;
  std::vector <struct SymbolTable*> childTables;
} TSymbolTable;

typedef struct
{
  TSymbolTable* table;
  unsigned index;
} TSymbolTableElement, *TSymbolTableElementPtr;

TSymbolTableElementPtr LookupUserVariableTable(TSymbolTable*, std::string);
TSymbolTableElementPtr LookupUserVariableTableRecursive(TSymbolTable*, std::string);
bool InsertUserVariableTable(TSymbolTable*, std::string, SubexpressionValueTypeEnum, TSymbolTableElementPtr&);
TSymbolTable* CreateUserVariableTable(TSymbolTable*);
bool HideUserVariableTable(TSymbolTable*);
void DestroyUserVariableTable(TSymbolTable*);
#endif // SYMTABLE_H
