#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The AssignmentNode class
 */
class AssignmentNode : public AbstractASTNode
{
public:
    AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value);

    void printNode(int level);
    AbstractSymbolTableRecord* getVariable(){
        return _variable;
    }
    AbstractASTNode* getValue(){
        return _value;
    }

    ~AssignmentNode();

private:
    AbstractSymbolTableRecord *_variable;
    AbstractASTNode *_value;
};


#endif // ASSIGNMENTNODE_H
