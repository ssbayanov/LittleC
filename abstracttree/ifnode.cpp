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

    QString trueTmp = "";
    QString falseTmp = "";
    QString endTmp = "";
    QString condTmp = _condition->printTripleCode();

    ir.startStore();

    //true branche
    ir.writeLabelLine();
    trueTmp = ir.lastCommandLine();
    if (_trueBranch != NULL) {
        _trueBranch->printTripleCode();
        ir.writeLine("br label %endLabel");
    }

    //false branche
    ir.writeLabelLine();
    falseTmp = ir.lastCommandLine();
    if (_falseBranch) {
        _falseBranch->printTripleCode();
        ir.writeLine("br label %endLabel");
    }

    //end
    ir.writeLabelLine();
    endTmp = ir.lastCommandLine();

    ir.stopStore();

    ir.replaceInStored("%endLabel", endTmp);

    ir.writeLine(QString("br i1 %1, label %2, label %3")
                 .arg(condTmp)
                 .arg(trueTmp)
                 .arg(falseTmp));

    ir.flushStore();

    return "";
}

IfNode::~IfNode()
{
    _condition->~AbstractASTNode();
    _trueBranch->~AbstractASTNode();
    if (_falseBranch != NULL)
        _falseBranch->~AbstractASTNode();
}
