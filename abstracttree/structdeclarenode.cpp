#include "structdeclarenode.h"
#include "listnode.h"

StructDeclareNode::StructDeclareNode(AbstractSymbolTableRecord *variable, AbstractASTNode *variablesList)
    : AbstractValueASTNode(NT_StructTypeDeclare)
{
    _variablesList = variablesList;
    _variable = variable;
}

void StructDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Declaration struct type: %1\n")
                 .arg(_variable->getName());

    if (_variablesList != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << "Variables:\n";
        _variablesList->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                  << "BAD VARIABLE LIST NODE!!!\n";
    }
}

QString StructDeclareNode::printTripleCode()
{
    _variable->setUniqueName(ir.getUniqueNameAndStore(_variable->getName()));
    QString vars = "";
    if(_variablesList->getType() == NT_List) {
        ((ListNode *)_variablesList)->setListType(LT_DeclareStructVars);
        vars = _variablesList->printTripleCode();
    }
    else {
        vars = ((AbstractValueASTNode *)_variablesList)->getValueTypeLLVM();
    }

    ir.writeGlobalLine(QString("%struct.%1 = type {%2}")
                       .arg(_variable->getUniqueName())
                       .arg(vars));
    return "";
}

StructDeclareNode::~StructDeclareNode()
{
    if (_variablesList != NULL)
        _variablesList->~AbstractASTNode();
}

