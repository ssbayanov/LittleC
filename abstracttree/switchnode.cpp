#include "switchnode.h"
#include "casenode.h"
#include "listnode.h"

SwitchNode::SwitchNode(AbstractASTNode *value, AbstractASTNode *caseList)
    : AbstractASTNode(NT_SwitchStatement)
{
    _value = value;
    _caseList = caseList;
}

void SwitchNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << "Switch:\n";

    if(_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Value:\n";
        _value->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD VALUE NODE\n";
    }
    if(_caseList != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Case list:\n";
        _caseList->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD CASE LIST NODE\n";
    }
}

QString SwitchNode::printTripleCode()
{
    QString value = _value->printTripleCode();

    ir.startStore();

    if(_caseList->getType() == NT_CaseStatement){
        CaseNode *caseNode = ((CaseNode *) _caseList);
        caseNode->printValues();
        caseNode->printTripleCode();
    }
    else {
        ((ListNode *)_caseList)->setListType(LT_CaseValueList);
        _caseList->printTripleCode();
        ((ListNode *)_caseList)->setListType(LT_CaseList);
        _caseList->printTripleCode();
    }

    ir.replaceInStored("$end$", ir.writeLabelLine("EndSwitch"));
    ir.replaceInStored("$value$", value);

    ir.flushStore();

    return "";
}

SwitchNode::~SwitchNode()
{
    if(_value != NULL)
        _caseList->~AbstractASTNode();
    if(_caseList != NULL)
        _value->~AbstractASTNode();
}


