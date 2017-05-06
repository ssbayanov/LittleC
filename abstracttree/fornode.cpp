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
        _init->printTripleCode(level);
    }

    ir.startStore();

    ir.writeLine(QString("br label %begin%"));
    ir.writeNamedLabelLine("Begin");

    if(_condition != NULL){
        ir.writeLine(QString("br i1 %1, label %body%, label %end%")
                     .arg(_condition->printTripleCode())
                     .arg("")
                     .arg(""));
    }
    else {
        ir.writeLine("br label %body%Loop");
    }

    ir.writeNamedLabelLine("Body");

    if(_body != NULL){
        _body->printTripleCode(level);
    }

    ir.writeLine(QString("br label %continue%"));
    ir.writeNamedLabelLine("Continue");

    if(_iteration != NULL){
        _iteration->printTripleCode(level);
    }    

    ir.writeLine(QString("br label %1")
                 .arg(ir.getLabelByName("Begin")));

    ir.writeNamedLabelLine("End");

    ir.stopStore();

    ir.replaceInStored("%body%", ir.getLabelByName("Body"));
    ir.replaceInStored("%end%", ir.getLabelByName("End"));
    ir.replaceInStored("%begin%", ir.getLabelByName("Begin"));
    ir.replaceInStored("%continue%", ir.getLabelByName("Continue"));

    ir.flushStore();



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

