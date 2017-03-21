#include "switchnode.h"

SwitchNode::SwitchNode(QString key, AbstractASTNode *value, AbstractASTNode *caseList)
    : AbstractASTNode(NT_SwitchStatement)
{
    _value = value;
    _caseList = caseList;
    _key = key;
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

QString SwitchNode::printTripleCode(int level, QString param)
{
    outStream << QString("\tvalue_%1 = %2\n")
                 .arg(_key)
                 .arg(_value->printTripleCode(level+1));

    if(_caseList != NULL){        
        _caseList->printTripleCode(level, "value");
        _caseList->printTripleCode(level, param);
    }

    outStream << QString("LoopEnd_%1:\n").arg(_key);

    return "";
}

SwitchNode::~SwitchNode()
{
    if(_value != NULL)
        _caseList->~AbstractASTNode();
    if(_caseList != NULL)
        _value->~AbstractASTNode();
}


