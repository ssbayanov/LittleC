#include "arrayassignmentnode.h"
#include "abstractvalueastnode.h"
#include "symbolstable/arraytablerecord.h"

ArrayAssignmentNode::ArrayAssignmentNode(AbstractSymbolTableRecord *array, AbstractASTNode *index, AbstractASTNode *value)
    : AbstractASTNode(NT_ArrayAssignment)
{
    _array = array;
    _index = index;
    _value = value;
}

void ArrayAssignmentNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Array Reference: %1, type: %2\n")
                  .arg(_array->getName())
                  .arg(typeName.at(_array->getValueType()));

    if (_index != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Index:\n";
        _index->printNode(level + 1);
    }
    else {
        treeStream << "BAD INDEX NODE!!!";
    }

    if (_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Value:\n";
        _value->printNode(level + 1);
    }
    else {
        treeStream << "BAD VALUE NODE!!!";
    }
}

QString ArrayAssignmentNode::printTripleCode()
{
    ir.writeCommandLine( QString("getelementptr inbounds [ %1 x %2 ], [ %1 x %2 ]* %5%3, i32 0, i32 %4")
                         .arg(((ArrayTableRecord*)_array)->getSize())
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg(_array->getUniqueName())
                         .arg(_index->printTripleCode())
                         .arg(_array->isGlobal() ? "@" : "%")
                         );
    ir.writeLine(QString("store %1 %2, %1* %3")
                        .arg(((AbstractValueASTNode*)_value)->getValueTypeLLVM())
                        .arg(_value->printTripleCode())
                        .arg(ir.lastCommandLine())
                        );

    return "";
}

