#include "casenode.h"

CaseNode::CaseNode(AbstractASTNode *value, AbstractASTNode *statements)
    : AbstractASTNode(NT_CaseStatement)
{
    _value = value;
    _statements = statements;
}

void CaseNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("%1:").arg(_value != NULL ? "case" : "default")
              << "\n";

    if(_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Value:\n";
        _value->printNode(level+1);
    }

    if(_statements != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Statements:\n";
        _statements->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD STATEMENTS NODE!!!\n";
    }
}

CaseNode::~CaseNode()
{
    if(_statements != NULL)
        _statements->~AbstractASTNode();
    if(_value != NULL)
        _value->~AbstractASTNode();
}
