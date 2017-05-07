#include "casenode.h"
#include "abstractvalueastnode.h"

CaseNode::CaseNode(AbstractASTNode *value, AbstractASTNode *statements)
    : AbstractASTNode(NT_CaseStatement)
{
    _value = value;
    _statements = statements;
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

QString CaseNode::printTripleCode()
{
    QString caseLabel = ir.writeLabelLine(QString("Case node %1")
                                          .arg(valuePtr));
    ir.replaceInStored(QString("$labelTo%1$")
                       .arg(valuePtr), caseLabel);

    _statements->printTripleCode();

    return "";
}

QString CaseNode::printValues()
{
    if(_value != NULL) {
        AbstractValueASTNode *value = ((AbstractValueASTNode *) _value);

        valuePtr = ir.writeCommandLine(QString("%1 %2 $value$, %4")
                                       .arg(isInt(value->getValueType()) ? "icmp eq" : "fcmp oeq")
                                       .arg(value->getValueTypeLLVM())
                                       .arg(value->printTripleCode()));

    ir.writeLine(QString("br i1 %1, label $labelTo%1$, label $next$")
                 .arg(valuePtr));

    ir.replaceInStored("$next$", ir.writeLabelLine("Next"));

    }
    else {
        valuePtr = "Default";
        ir.writeLine("br label $labelToDefault$");
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
