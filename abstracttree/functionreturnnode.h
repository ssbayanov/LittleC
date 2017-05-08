#ifndef FUNCTIONRETURNNODE_H
#define FUNCTIONRETURNNODE_H

#include "abstracttree/abstractvalueastnode.h"

class FunctionReturnNode : public AbstractValueASTNode
{
public:
      FunctionReturnNode(AbstractASTNode *expression);

      void printNode(int level);

      QString printTripleCode();

      ~FunctionReturnNode();
private:
      AbstractASTNode *_expression;
};

#endif // FUNCTIONRETURNNODE_H
