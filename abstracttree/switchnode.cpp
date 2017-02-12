#include "switchnode.h"

SwitchNode::SwitchNode(AbstractASTNode *value, AbstractASTNode *caseList)
    : AbstractASTNode(NT_SwitchStatement)
{
    _value = value;
    _caseList = caseList;
}

void SwitchNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << "Switch:"
              << std::endl;

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Value:"
              << std::endl;
    _value->printNode(level+1);

    std::cout << QString().fill(' ',level*2).toStdString()
              << "Case list:"
              << std::endl;
    _caseList->printNode(level+1);
}

SwitchNode::~SwitchNode()
{
    _caseList->~AbstractASTNode();
    _value->~AbstractASTNode();
}


