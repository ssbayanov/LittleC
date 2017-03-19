#include "fornode.h"

ForNode::ForNode(QString key, AbstractASTNode *init, AbstractASTNode *condition, AbstractASTNode *iteration, AbstractASTNode *body)
    : AbstractASTNode(NT_ForStatement)
{
    _init = init;
    _condition = condition;
    _iteration = iteration;
    _body = body;
    _key = key;
}

void ForNode::setBody(AbstractASTNode *body)
{
    _body = body;
}

void ForNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << "For:\n";

    if(_init != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Initialisation:\n";
        _init->printNode(level+1);
    }

    if(_condition != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Condition:\n";
        _condition->printNode(level+1);
    }

    if(_iteration != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Increment:\n";
        _iteration->printNode(level+1);
    }

    if(_body != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Body:\n";
        _body->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD BODY NODE!!!\n";
    }
}

QString ForNode::printTripleCode(int level, QString param)
{
    if(_init != NULL) {
        _init->printTripleCode(level+1);
    }

    outStream << QString("LoopBegin_%1:\n")
                 .arg(_key);

    if(_condition != NULL){
        outStream << QString("iffalse %1 goto LoopEnd_%2\n")
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

    return "";

}

ForNode::~ForNode()
{
    if(_init != NULL)
        _init->~AbstractASTNode();
    if(_condition != NULL)
        _condition->~AbstractASTNode();
    if(_iteration != NULL)
        _iteration->~AbstractASTNode();
    if(_body != NULL)
        _body->~AbstractASTNode();
}

