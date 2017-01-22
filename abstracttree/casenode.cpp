#include "casenode.h"

CaseNode::CaseNode(AbstractASTNode *value, AbstractASTNode *statements)
    : AbstractASTNode(NT_CaseStatement)
{
    _value = value;
    _statements = statements;
}

void CaseNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("%1:").arg(_value != NULL ? "case" : "default").toStdString()
              << std::endl;

    if(_value != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Value:"
                  << std::endl;
        _value->printNode(level+1);
    }

    if(_statements != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Statements:"
                  << std::endl;
        _statements->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD STATEMENTS NODE!!!"
                  << std::endl;
    }
}

CaseNode::~CaseNode()
{
    if(_statements != NULL)
        _statements->~AbstractASTNode();
    if(_value != NULL)
        _value->~AbstractASTNode();
}
