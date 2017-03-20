#include "casenode.h"

CaseNode::CaseNode(QString key, AbstractASTNode *value, AbstractASTNode *statements)
    : AbstractASTNode(NT_CaseStatement)
{
    _value = value;
    _statements = statements;
    _key = key;
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

QString CaseNode::printTripleCode(int level, QString param)
{
    if(param == "value"){
        if(_value != NULL){
            outStream << QString("$t%1 = value_%2 == %3\n")
                         .arg(level)
                         .arg(_key)
                         .arg(_value->printTripleCode(level+1));


            outStream << QString("if $t%1 goto CaseLabel_%1_%2\n")
                         .arg(level)
                         .arg(_key);
        }
        else {
            outStream << QString("goto CaseLabel_%1_%2\n")
                         .arg(level)
                         .arg(_key);
        }
    }
    else {
        if(_statements != NULL){
            outStream << QString("CaseLabel_%1_%2:\n")
                         .arg(level)
                         .arg(_key);
            _statements->printTripleCode();
        }
    }

    return "";
}

CaseNode::~CaseNode()
{
    if(_statements != NULL)
        _statements->~AbstractASTNode();
    if(_value != NULL)
        _value->~AbstractASTNode();
}
