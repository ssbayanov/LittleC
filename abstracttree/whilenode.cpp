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
    ir.startStore();

    ir.writeLine("br label %continue");
    ir.writeNamedLabelLine("Continue");

    ir.writeLine("br label %begin");
    ir.writeNamedLabelLine("Begin");

    if(!_isDoWhile){
        if(_condition != NULL){
            ir.writeLine(QString("br i1 %1, label %body, label %end")
                         .arg(_condition->printTripleCode(level+1)));
        }
        else {
            ir.writeLine("br label %body");
        }
    }


    ir.writeNamedLabelLine("Body");
    if(_body != NULL){
        _body->printTripleCode();

    }

    if(_isDoWhile) {
        if(_condition != NULL){
            ir.writeLine(QString("br i1 %1 label %body, label %end")
                         .arg(_condition->printTripleCode(level+1)));
        }

    }

    ir.writeLine("br label %begin");
    ir.writeNamedLabelLine("End");

    ir.stopStore();

    ir.replaceInStored("%body", ir.getLabelByName("Body"));
    ir.replaceInStored("%end", ir.getLabelByName("End"));
    ir.replaceInStored("%begin", ir.getLabelByName("Begin"));
    ir.replaceInStored("%continue", ir.getLabelByName("Continue"));

    ir.flushStore();

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

