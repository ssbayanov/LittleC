#include "assignmentnode.h"

AssignmentNode::AssignmentNode(AbstractSymbolTableRecord *variable, AbstractASTNode *value, AbstractASTNode *index)
    :AbstractASTNode(NT_AssignmentOperation)
{
    _variable = variable;
    _value = value;
    _index = index;
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

QString AssignmentNode::printTripleCode()
{
    QString LCL;
    if(_variable->isArray()){
        if(_value->getType() == NT_ArrayReference){
            ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
                          .arg(((arraytablerecord*)(((ArrayReferenceNode*)_value)->getId()))->getSize())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(((arraytablerecord*)(((ArrayReferenceNode*)_value)->getId()))->getSize())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(((ArrayReferenceNode*)_value)->getId()->getName())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(0)
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(((ArrayReferenceNode*)_value)->getIndex()->printTripleCode()));
            ir.writeCommandLine(QString("load %1, %2* %3")
                         .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                         .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                         .arg(ir.lastCommandLine()));

            if(_index->getType() == NT_NumericConstant) {
                if(((ValueNode*)_index)->getValueType()==typeInt) {
             ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
                           .arg(((arraytablerecord*)_variable)->getSize())
                           .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                           .arg(((arraytablerecord*)_variable)->getSize())
                           .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                           .arg(_variable->getName())
                           .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                           .arg(0)
                           .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                           .arg(((ValueNode*)_index)->getValue().toInt()));
             ir.writeLine(QString("store %1 %2, %3* %4")
                          .arg(((AbstractValueASTNode * )_value)->getValueTypeLLVM())
                          .arg(_value->printTripleCode())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                          .arg(ir.lastCommandLine()));
                }
            }
            else {

            }
        }
        else {
//       if(_index->getType() == NT_NumericConstant) {
//           if(((ValueNode*)_index)->getValueType()==typeInt) {
//        ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
//                      .arg(((arraytablerecord*)_variable)->getSize())
//                      .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
//                      .arg(((arraytablerecord*)_variable)->getSize())
//                      .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
//                      .arg(_variable->getName())
//                      .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
//                      .arg(0)
//                      .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
//                      .arg(((ValueNode*)_index)->getValue().toInt()));
//        ir.writeLine(QString("store %1 %2, %3* %4")
//                     .arg(((AbstractValueASTNode * )_value)->getValueTypeLLVM())
//                     .arg(_value->printTripleCode())
//                     .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
//                     .arg(ir.lastCommandLine()));
//           }
//       }
//       else {

//       }
           // %5 = getelementptr inbounds [3 x i32], [3 x i32]* %f, i64 0, i64 1
            //store i32 5, i32* %5, align 4
                    ir.writeCommandLine( QString("getelementptr inbounds [ %1 x %2 ], [ %1 x %2 ]* %%3, i32 0, i32 %4")
                    .arg(((arraytablerecord*)_variable)->getSize())
                    .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                    .arg(_variable->getName())
                    .arg(_index->printTripleCode())
                    );
                    ir.writeCommandLine(QString("store %1 %2, %1* %3")
                    .arg(AbstractValueASTNode::getValueTypeLLVM(((AbstractValueASTNode*)_value)->getValueType()))
                    .arg(_value->printTripleCode())
                    .arg(ir.lastCommandLine())
                    );
        }
    }
    else {
        if(_value->getType() == NT_ArrayReference){
            ir.writeCommandLine( QString("getelementptr [ %1 x %2 ], [ %3 x %4 ]* %%5, %6 %7, %8 %9")
                          .arg(((arraytablerecord*)(((ArrayReferenceNode*)_value)->getId()))->getSize())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(((arraytablerecord*)(((ArrayReferenceNode*)_value)->getId()))->getSize())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(((ArrayReferenceNode*)_value)->getId()->getName())
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(0)
                          .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                          .arg(((ArrayReferenceNode*)_value)->getIndex()->printTripleCode()));
            ir.writeCommandLine(QString("load %1, %2* %3")
                         .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                         .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                         .arg(ir.lastCommandLine()));
            ir.writeLine(QString("store %1 %2, %3* %4")
                         .arg(AbstractValueASTNode::getValueTypeLLVM(((ArrayReferenceNode*)_value)->getId()->getValueType()))
                         .arg(ir.lastCommandLine())
                         .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                         .arg((_variable->isGlobal() ? "@" : "%")+_variable->getName()));
        }
        else {
    ir.writeLine(QString("store %1 %2, %3* %4")
                 .arg(((AbstractValueASTNode * )_value)->getValueTypeLLVM())
                 .arg(_value->printTripleCode())
                 .arg(AbstractValueASTNode::getValueTypeLLVM(_variable->getValueType()))
                 .arg((_variable->isGlobal() ? "@" : "%")+_variable->getName()));
    }
    }
    return "";
}

