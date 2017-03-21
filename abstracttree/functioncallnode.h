#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "abstracttree/abstractvalueastnode.h"
#include "symbolstable/abstractsymboltablerecord.h"

class FunctionCallNode : public AbstractValueASTNode
{
public:
      FunctionCallNode(AbstractSymbolTableRecord *variable, AbstractASTNode *paramList);

      void printNode(int level);

      QString printTripleCode(int level, QString param);

      ~FunctionCallNode();
private:
      AbstractSymbolTableRecord *_variable;
      AbstractASTNode *_paramList;
};

#endif // FUNCTIONCALLNODE_H
