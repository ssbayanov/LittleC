#include "whilenode.h"

WhileNode::WhileNode(QString key, AbstractASTNode *condition, AbstractASTNode *body)
    : AbstractASTNode(NT_WhileStatement)
{
    _condition = condition;
    _body = body;
    _isDoWhile = false;
    _key = key;
}

void WhileNode::setBody(AbstractASTNode *body)
{
    _body = body;
}

void WhileNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << (_isDoWhile == false ? "While:" : "Do-While")
               << "\n";


    if(!_isDoWhile && _condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Condition:\n";
        _condition->printNode(level+1);
    }

    if(_body != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Body:\n";
        _body->printNode(level+1);
    }

    if(_isDoWhile && _condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Condition:\n";
        _condition->printNode(level+1);
    }

    if (_condition == NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "BAD CONDITION NODE!!!\n";
    }
}

QString WhileNode::printTripleCode(int level, QString param)
{
    outStream << QString("LoopBegin_%1:\n").arg(_key);
    outStream << QString("LoopContinue_%1:\n").arg(_key);
    if(!_isDoWhile){
        if(_condition != NULL){
            outStream << QString("\tiffalse %1 goto LoopEnd_%2\n")
                         .arg(_condition->printTripleCode(level+1))
                         .arg(_key);
        }
    }

    if(_body != NULL){
        _body->printTripleCode();

    }

    if(_isDoWhile) {
        if(_condition != NULL){
            outStream << QString("\tiffalse %1 goto LoopBegin_%2\n")
                         .arg(_condition->printTripleCode(level+1))
                         .arg(_key);
        }
        else {
            outStream << QString("\tgoto LoopBegin_%1\n")
                         .arg(_key);
        }
    }

    outStream << QString("LoopEnd_%1:\n").arg(_key);

    return "";
}

void WhileNode::setIsDoWhile(bool isDoWhile)
{
    _isDoWhile = isDoWhile;
}

WhileNode::~WhileNode()
{
    if(_condition != NULL)
        _condition->~AbstractASTNode();
    if(_body != NULL)
        _body->~AbstractASTNode();
}

