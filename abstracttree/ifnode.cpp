#include "ifnode.h"

IfNode::IfNode(QString key, AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch)
    : AbstractASTNode(NT_IfStatement)
{
    _condition = condition;
    _trueBranch = trueBranch;
    _falseBranch = falseBranch;
    _key = key;
}

void IfNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << "If\n";
    if (_condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Condition:\n";
        _condition->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD CONDITION NODE!!!\n";
    }

    if (_trueBranch != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "True branch:\n";
        _trueBranch->printNode(level+1);
    }

    if (_falseBranch != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "False branch:\n";
        _falseBranch->printNode(level+1);
    }
}

QString IfNode::printTripleCode(int level, QString param)
{

    if(_condition != NULL){
        outStream << QString("\tiffalse %1 goto Else_%2\n")
                     .arg(_condition->printTripleCode(level+1))
                     .arg(_key);
    }

    if(_trueBranch != NULL){
        _trueBranch->printTripleCode(level+1);

    }

    outStream << QString("\tgoto EndIf_%1\n")
                 .arg(_key);
    outStream << QString("Else_%1:\n")
                 .arg(_key);

    if(_falseBranch != NULL){
        _falseBranch->printTripleCode(level+1);

    }

    outStream << QString("EndIf_%1:\n")
                 .arg(_key);

    return "";
}

IfNode::~IfNode()
{
    _condition->~AbstractASTNode();
    _trueBranch->~AbstractASTNode();
    if (_falseBranch != NULL)
        _falseBranch->~AbstractASTNode();
}
