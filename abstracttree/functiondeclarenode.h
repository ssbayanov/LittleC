#ifndef FUNCTIONDECLARENODE_H
#define FUNCTIONDECLARENODE_H

#include "abstracttree/abstractvalueastnode.h"
#include "symbolstable/abstractsymboltablerecord.h"

class FunctionDeclareNode : public AbstractValueASTNode
{
public:
      FunctionDeclareNode(AbstractSymbolTableRecord *variable, AbstractASTNode *paramList, AbstractASTNode *body);

      void printNode(int level);

      ~FunctionDeclareNode();
private:
      AbstractSymbolTableRecord *_variable;
      AbstractASTNode *_paramList;
      AbstractASTNode *_body;
};

#endif // FUNCTIONDECLARENODE_H
