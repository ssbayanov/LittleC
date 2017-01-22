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

    if(_value != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Value:"
                  << std::endl;
        _value->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD VALUE NODE"
                  << std::endl;
    }
    if(_caseList != NULL) {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "Case list:"
                  << std::endl;
        _caseList->printNode(level+1);
    }
    else {
        std::cout << QString().fill(' ',level*2).toStdString()
                  << "BAD CASE LIST NODE"
                  << std::endl;
    }
}

SwitchNode::~SwitchNode()
{
    if(_value != NULL)
        _caseList->~AbstractASTNode();
    if(_caseList != NULL)
        _value->~AbstractASTNode();
}


