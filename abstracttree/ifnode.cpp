#include "ifnode.h"

IfNode::IfNode(AbstractASTNode *condition, AbstractASTNode *trueBranch, AbstractASTNode *falseBranch)
    : AbstractASTNode(NT_IfStatement)
{
    _condition = condition;
    _trueBranch = trueBranch;
    _falseBranch = falseBranch;
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

    /*if(_condition != NULL){
        outStream << QString("iffalse %1 goto Else_%2\n")
                     .arg(_condition->printTripleCode(level+1))
                     .arg(_key);
    }

    if(_body != NULL){
        _body->printTripleCode(level+1);

    }

    outStream << QString("LoopContinue_%1:\n").arg(_key);

    if(_iteration != NULL){
        _iteration->printTripleCode(level+1);
    }

    outStream << QString("goto LoopBegin_%1\n")
                 .arg(_key);

    outStream << QString("LoopEnd_%1:\n")
                 .arg(_key);
                 */

    return "";
}

IfNode::~IfNode()
{
    _condition->~AbstractASTNode();
    _trueBranch->~AbstractASTNode();
    if (_falseBranch != NULL)
        _falseBranch->~AbstractASTNode();
}
