#include "structvariableassignment.h"
#include "symbolstable/structtablerecord.h"

StructVariableAssignment::StructVariableAssignment(AbstractSymbolTableRecord *structVariable,
                                                   AbstractSymbolTableRecord *variable,
                                                   AbstractASTNode *value)
    : AbstractValueASTNode(NT_StructAssignment)
{
    _structVariable = structVariable;
    _variable = variable;
    _typeValue = variable->getValueType();
    _value = value;
}

void StructVariableAssignment::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Struct var assignment: %1.%2, type: %3\n")
                 .arg(_structVariable->getName())
                 .arg(_variable->getName())
                 .arg(typeName.at(_variable->getValueType()));

    if(_value != NULL) {
        treeStream << QString().fill(' ',level*2)
                  << QString("Value");
        _value->printNode(level+1);
    }

}

QString StructVariableAssignment::printTripleCode()
{
    QString structVarPtr = ir.writeCommandLine(QString("getelementptr inbounds %struct.%1, %struct.%1* %%2, i32 0, i32 %3")
                        .arg(_structVariable->getTypeName())
                        .arg(_structVariable->getName())
                        .arg(((StructTableRecord *)_structVariable)->getVarIndex(_variable)));
    QString newValue;
    if(_value != NULL) {
        newValue =_value->printTripleCode();
    }

    ir.writeLine(QString("store %1 %2, %1* %3")
                 .arg(getValueTypeLLVM())
                 .arg(newValue)
                 .arg(structVarPtr));

    return "";
}

