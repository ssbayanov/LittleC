#include "whilenode.h"

WhileNode::WhileNode(AbstractASTNode *condition, AbstractASTNode *body)
    : AbstractASTNode(NT_WhileStatement)
{
    _condition = condition;
    _body = body;
    _isDoWhile = false;
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

QString WhileNode::printTripleCode(int level)
{

if(_isDoWhile) goto m2; else goto m1;
 m1:   if(_condition != NULL){
        outStream << QString("L%1:\n").arg(g_LastLabelNumber);
        PushLabelNumber(g_LastLabelNumber);
        g_LastLabelNumber++;

        outStream << QString("iffalse %1 goto L%2\n")
                     .arg(_condition->printTripleCode(level+1))
                     .arg(g_LastLabelNumber);
        g_LastLabelNumber++;
        }
m2:    if(_body != NULL){

         if(_isDoWhile) {
          outStream << QString("L%1:\n")
                          .arg(g_LastLabelNumber);
          PushLabelNumber(g_LastLabelNumber);
          g_LastLabelNumber++;
         }

        _body->printTripleCode();
 //       PopLabelNumber();


       int tmp = PopLabelNumber();
        outStream << QString("goto L%1\n")
                     .arg(tmp);
        if(_isDoWhile) {
            PushLabelNumber(g_LastLabelNumber-1);
            outStream << QString("L%1:\n")
                         .arg(g_LastLabelNumber);
        }
        else {
        PushLabelNumber(++tmp);
        outStream << QString("L%1:\n")
                     .arg(tmp);
       }

    }
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

