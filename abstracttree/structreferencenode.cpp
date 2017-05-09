#include "structreferencenode.h"
#include "symbolstable/structtablerecord.h"

StructReferenceNode::StructReferenceNode(AbstractSymbolTableRecord *structVariable, AbstractSymbolTableRecord *variable)
    : AbstractValueASTNode(NT_StructReference)
{
    _structVariable = structVariable;
    _variable = variable;
    _typeValue = variable->getValueType();
}

void StructReferenceNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Struct reference: %1.%2, type: %3\n")
                  .arg(_structVariable->getName())
                  .arg(_variable->getName())
                  .arg(typeName.at(_variable->getValueType()));
}

QString StructReferenceNode::printTripleCode()
{
    QString valuePtr = ir.writeCommandLine(QString("getelementptr inbounds %struct.%1, %struct.%1* %%2, i32 0, i32 %3")
                                           .arg(_structVariable->getTypeName())
                                           .arg(_structVariable->getUniqueName())
                                           .arg(((StructTableRecord *)_structVariable)->getVarIndex(_variable)));
    return ir.writeCommandLine(QString("load %1, %1* %2")
                               .arg(getValueTypeLLVM())
                               .arg(valuePtr));
}

StructReferenceNode::~StructReferenceNode()
{

}
