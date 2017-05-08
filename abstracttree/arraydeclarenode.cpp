#include "arraydeclarenode.h"

ArrayDeclareNode::ArrayDeclareNode(AbstractSymbolTableRecord *array, AbstractASTNode *values)
    : AbstractASTNode(NT_ArrayDeclare)
{
    _array = array;
    _values = values;

    if(values->getType() == NT_NumericConstant){
        if(((ValueNode*)_values)->getValueType()==typeInt){
            ((arraytablerecord*)array)->setSize(((ValueNode*)_values)->getValue().toInt());
        }
        else if(((ValueNode*)_values)->getValueType()==typeString) {
            ((arraytablerecord*)array)->setSize(((ValueNode*)_values)->getValue().toString().size()+1);
        }

    }
    else if(values->getType() == NT_List){
        ((ListNode*)values)->setListType(LT_ArrayValueList);
         ((ListNode*)values)->setSize();
        ((arraytablerecord*)array)->setSize(((ListNode*)values)->getSize());

    }
    else {

       printf("Inadmissible value type for Array \n");
    }

}


QString ArrayDeclareNode::printTripleCode()
{
    AbstractASTNode *tmp;
    tmp = (ListNode*)_values;
    if(_array->isGlobal()) {
        ir.writeLine( QString("@%1 = global [ %3 x %2 ]")
                      .arg(_array->getName())
                      .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                      .arg(((arraytablerecord*)_array)->getSize()));
      if(_values->getType() == NT_NumericConstant){
       if(((ValueNode*)_values)->getValueType()==typeString) {
           for(int i = 0;i<((arraytablerecord*)_array)->getSize()-1;i++){
                ir.writeLine(QString("store %1 %2, %3* getelementptr [ %4 x %5 ], [ %6 x %7 ]* @%8, %9 %10, %11 %12")
                             .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                             .arg(((ValueNode*)_values)->getValue().toString().at(i))
                             .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                        .arg(((arraytablerecord*)_array)->getSize())
                        .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                        .arg(((arraytablerecord*)_array)->getSize())
                        .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                        .arg(_array->getName())
                        .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                        .arg(0)
                        .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                        .arg(i));
           }
           ir.writeLine(QString("store %1 %2, %3* getelementptr [ %4 x %5 ], [ %6 x %7 ]* @%8, %9 %10, %11 %12")
                        .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                        .arg("\0")
                        .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                   .arg(((arraytablerecord*)_array)->getSize())
                   .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                   .arg(((arraytablerecord*)_array)->getSize())
                   .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                   .arg(_array->getName())
                   .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                   .arg(0)
                   .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                   .arg(((arraytablerecord*)_array)->getSize()-1));
        }
     }
     else if(_values->getType() == NT_List){
          for(int i = 0;i<((arraytablerecord*)_array)->getSize();i++){
               ir.writeLine(QString("store %1 %2, %3* getelementptr [ %4 x %5 ], [ %6 x %7 ]* @%8, %9 %10, %11 %12")
                            .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                            .arg(tmp->isValueNode() ? tmp->printTripleCode() : ((ListNode*)tmp)->getLeftNode()->printTripleCode())
                            .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                       .arg(((arraytablerecord*)_array)->getSize())
                       .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                       .arg(((arraytablerecord*)_array)->getSize())
                       .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                       .arg(_array->getName())
                       .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                       .arg(0)
                       .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                       .arg(i));
               if(!tmp->isValueNode()) tmp = ((ListNode*)tmp)->getRightNode();
          }
     }
    }
    else {
        ir.writeLine( QString("%%1 = alloca [ %3 x %2 ]")
                      .arg(_array->getName())
                      .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                      .arg(((arraytablerecord*)_array)->getSize()));
       if(_values->getType() == NT_NumericConstant){
        if(((ValueNode*)_values)->getValueType()==typeString) {
            for(int i = 0;i<((arraytablerecord*)_array)->getSize()-1;i++){
                 ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
                          .arg(((arraytablerecord*)_array)->getSize())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                          .arg(((arraytablerecord*)_array)->getSize())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                          .arg(_array->getName())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                          .arg(0)
                          .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                          .arg(i));

                 ir.writeLine( QString("store %1 %2, %3* %4")
                              .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                              .arg(((ValueNode*)_values)->getValue().toString().at(i))
                              .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                              .arg(ir.lastCommandLine()));
            }

            ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
                     .arg(((arraytablerecord*)_array)->getSize())
                     .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                     .arg(((arraytablerecord*)_array)->getSize())
                     .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                     .arg(_array->getName())
                     .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                     .arg(0)
                     .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                     .arg(((arraytablerecord*)_array)->getSize()-1));
            ir.writeLine(QString("store %1 %2, %3* %4")
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg("\0")
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg(ir.lastCommandLine()));
         }
       }
       else if(_values->getType() == NT_List) {
           for(int i = 0;i<((arraytablerecord*)_array)->getSize(); i++){
                ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
                         .arg(((arraytablerecord*)_array)->getSize())
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg(((arraytablerecord*)_array)->getSize())
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg(_array->getName())
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg(0)
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                         .arg(i));
                ir.writeLine( QString("store %1 %2, %3* %4")
                             .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                             .arg(tmp->isValueNode() ? tmp->printTripleCode() : ((ListNode*)tmp)->getLeftNode()->printTripleCode())
                             .arg(AbstractValueASTNode::getValueTypeLLVM(_array->getValueType()))
                             .arg(ir.lastCommandLine()));

                if(!tmp->isValueNode()) tmp = ((ListNode*)tmp)->getRightNode();
            }
       }
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
