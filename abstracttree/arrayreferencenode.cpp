#include "arrayreferencenode.h"
#include "symbolstable/arraytablerecord.h"

ArrayReferenceNode::ArrayReferenceNode(AbstractSymbolTableRecord *array, AbstractValueASTNode *index)
    : AbstractValueASTNode(NT_ArrayReference)
{
    _array = array;
    _index = index;
    _typeValue = array->getValueType();
}

void ArrayReferenceNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Array Reference: %1, type: %2\n").arg(_array->getName()).arg(typeName.at(getValueType()));

    if (_index != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Index:\n";
        _index->printNode(level + 1);
    }
    else {
        treeStream << "BAD INDEX NODE!!!";
    }
}

QString ArrayReferenceNode::printTripleCode()
{
    ir.writeCommandLine( QString("getelementptr inbounds [ %1 x %2 ], [ %1 x %2 ]* %5%3, i32 0, i32 %4")
                         .arg(((arraytablerecord*)_array)->getSize())
                         .arg(getValueTypeLLVM())
                         .arg(_array->getName())
                         .arg(_index->printTripleCode())
                         .arg(_array->isGlobal() ? "@" : "%"));

    return ir.writeCommandLine(QString("load %1, %1* %2")
                               .arg(getValueTypeLLVM())
                               .arg(ir.lastCommandLine()));
}

ArrayReferenceNode::~ArrayReferenceNode()
{
    if (_index != NULL) {
        _index->~AbstractASTNode();
    }
}

