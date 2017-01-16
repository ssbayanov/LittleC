#include "casenode.h"

CaseNode::CaseNode(AbstractASTNode *value, AbstractASTNode *expressions)
    : AbstractASTNode(NT_CaseStatement)
{
    _value = value;
    _expressions = expressions;
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
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Expressions:"
              << std::endl;
    _expressions->printNode(level+1);
}

CaseNode::~CaseNode()
{
    _expressions->~AbstractASTNode();
    _value->~AbstractASTNode();
}
