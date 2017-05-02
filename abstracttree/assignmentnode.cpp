#include "assignmentnode.h"

AssignmentNode::AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value)
    :AbstractASTNode(NT_AssignmentOperation)
{
    _variable = variable;
    _value = value;
}

void AssignmentNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Assignment operation: %1, type: %2\n")
                  .arg(_variable->getName())
                  .arg(_variable->getValueType());

    if (_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Value:\n";
        _value->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                   << "BAD VALUE NODE!!!\n";
    }
}

QString AssignmentNode::printTripleCode(int level, QString param)
{
    ir.writeLine(QString("store %1 %2, %3* %4")
                 .arg(((AbstractValueASTNode * )_value)->getValueTypeLLVM())
                 .arg(_value->printTripleCode())
                 .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                 .arg((_variable->isGlobal() ? "@" : "%")+_variable->getName()));
    return "";
}

