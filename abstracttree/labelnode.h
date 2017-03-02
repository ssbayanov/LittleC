#ifndef LABELNODE_H
#define LABELNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The LabelNode class
 */
class LabelNode : public AbstractASTNode
{
public:
    LabelNode(AbstractSymbolTableRecord *variable);

    void printNode(int level);

    QString printTripleCode(int level);

    ~LabelNode();

private:
    AbstractSymbolTableRecord *_variable;
};


#endif // LABELNODE_H
