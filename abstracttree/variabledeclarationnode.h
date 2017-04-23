#ifndef VARIABLEDECLARATIONNODE_H
#define VARIABLEDECLARATIONNODE_H
#include "abstracttree/ast.h"


class VariableDeclarationNode : public AbstractValueASTNode
{
public:
    VariableDeclarationNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value = NULL);
    
    void printNode(int level);
    
    QString printTripleCode(int level, QString param);
    
    ~VariableDeclarationNode();
private:
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
};

#endif // VARIABLEDECLARATIONNODE_H
