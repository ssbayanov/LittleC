#include "structvariabledeclarenode.h"

StructVariableDeclareNode::StructVariableDeclareNode(AbstractSymbolTableRecord *typeStruct, AbstractSymbolTableRecord *variable)
    : AbstractValueASTNode(NT_StructDeclare)
{
    _typeStruct = typeStruct;
    _variable = variable;
}

void StructVariableDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Declaration struct: %1 type: %2\n")
                 .arg(_variable->getName())
                 .arg(_typeStruct->getName());
}

QString StructVariableDeclareNode::printTripleCode()
{
    ir.writeLine(QString("%%2 = alloca %struct.%1")
                 .arg(_typeStruct->getName())
                 .arg(_variable->getName()));
    return "";
}

StructVariableDeclareNode::~StructVariableDeclareNode()
{

}

