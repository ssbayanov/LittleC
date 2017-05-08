#include "arraydeclarenode.h"

ArrayDeclareNode::ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values, AbstractASTNode *size)
    : AbstractASTNode(NT_ArrayDeclare)
{
    _array = array;
    _values = values;
    _size = size;
    if(_values != NULL) {
        if(_values == NT_List) {
            ((arraytablerecord*)_array)->setSize(((ListNode *) _values)->getSize());
        }
        else {
            ((arraytablerecord*)_array)->setSize(1);
        }
    }
    else {
        ((arraytablerecord*)_array)->setSize(((ValueNode *)_size)->getValue().toInt());
    }

}


QString ArrayDeclareNode::printTripleCode()
{
    if(_array->isGlobal()){
        ir.writeLine(QString("@%1 = global [%2 x %3] zeroinitializer")
                     .arg(_array->getName())
                     .arg(_size->printTripleCode())
                     .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType())));
    }
    else {

        ir.writeLine(QString("%%1 = alloca [%2 x %3]")
                     .arg(_array->getName())
                     .arg(_size == NULL? ((ListNode *) _values)->getSize(): _size->printTripleCode())
                     .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType())));
    }

    if(_values != NULL) {
        printValues(0, _values);
    }

    return "";
}

void ArrayDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Array Declare: %1, type: %2\n")
                  .arg(_array->getName())
                  .arg(typeName.at(_array->getValueType()));

    if (_values != NULL) {
        treeStream
                << QString().fill(' ',level*2)
                << "Values:\n";
        _values->printNode(level + 1);
    }
    else {
        treeStream << "BAD VALUES NODE!!!\n";
    }
}

ArrayDeclareNode::~ArrayDeclareNode()
{
    if (_values != NULL) {
        _values->~AbstractASTNode();
    }
}

void ArrayDeclareNode::printAssign(uint index, AbstractASTNode value)
{
    ArrayAssignmentNode(_array, ValueNode(typeInt,index), value).printTripleCode();
}

void ArrayDeclareNode::printValues(uint index, AbstractASTNode value)
{
    if(value != NULL) {
        if(value == NT_List) {
            printValues(++index, ((ListNode *) value)->getRightNode());
            printValues(++index, ((ListNode *) value)->getLeftNode());
        }
        else {
            printAssign(index, value);
        }
    }
}
