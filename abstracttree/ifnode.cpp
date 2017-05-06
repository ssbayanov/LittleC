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

    ir.startStore();

    ir.writeLine(QString("br i1 %1, label %true%, label %2")
                 .arg(_condition->printTripleCode())
                 .arg( (_falseBranch != NULL) ? "%false%" : "%endif%"));

    //true branche
    ir.writeNamedLabelLine("True");

    if (_trueBranch != NULL) {
        _trueBranch->printTripleCode();
        ir.writeLine("br label %endif%");
    }

    //false branche

    if (_falseBranch != NULL) {
        ir.writeNamedLabelLine("False");
        _falseBranch->printTripleCode();
        ir.writeLine("br label %endif%");
    }

    //end
    ir.writeNamedLabelLine("EndIf");


    ir.replaceInStored("%endif%", ir.getLabelByName("EndIf"));
    ir.replaceInStored("%true%", ir.getLabelByName("True"));
    ir.replaceInStored("%false%", ir.getLabelByName("False"));

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
