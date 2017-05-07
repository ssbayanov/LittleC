#include "printnode.h"
#include "abstractvalueastnode.h"
#include "valuenode.h"

PrintNode::PrintNode(AbstractASTNode *expression)
    : AbstractASTNode(NT_PintStatement)
{
    _expression = expression;
}

void PrintNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << "Print:\n";
    if (_expression != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Expression:\n";
        _expression->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                   << "BAD EXPRESSION NODE!!!\n";
    }
}

QString PrintNode::printTripleCode()
{
    ir.addExternFunction("declare i32 @printf(i8*, ...)");

    QString valueString = "";
    QString printString = "i8* getelementptr inbounds ([%2 x i8], [%2 x i8]* %1, i32 0, i32 0)";

    if (_expression != NULL){
        if(_expression->isValueNode()) {
            AbstractValueASTNode *valueExpression = ((AbstractValueASTNode *) _expression);
            if(valueExpression->getValueType() == typeString){
                printString = printString
                        .arg(_expression->printTripleCode())
                        .arg(((ValueNode *) _expression)->getValue().toByteArray().length()+1);
            }
            else{
                QString stringType = "";
                switch (valueExpression->getValueType()) {
                case typeInt:
                case typeBool:
                case typeShort:
                    stringType = "%d";
                    break;
                case typeFloat:
                case typeDouble:
                    stringType = "%f";
                    break;
                default:
                    break;
                }

                printString = printString
                        .arg(printTextVariable(stringType))
                        .arg(stringType.length()+1);
                valueString = QString("%1 %2")
                        .arg(valueExpression->getValueTypeLLVM())
                        .arg(valueExpression->printTripleCode());
            }

        }
    }
    ir.writeCommandLine(QString("call i32 (i8*, ...) @printf(%1%2)")
                        .arg(printString)
                        .arg(valueString.isEmpty() ? "" : QString(", %1")
                                                     .arg(valueString)));
    return "";
}

PrintNode::~PrintNode()
{
    if (_expression != NULL)
        _expression->~AbstractASTNode();
}

QString PrintNode::printTextVariable(QString text)
{
    return ir.addUnnamedVariable(
                QString("private unnamed_addr constant [%1 x i8] c\"%2\\00\"")
                .arg(text.length()+1)
                .arg(text
                     .replace("\n","\\0A")
                     .replace("\t", "\\09")));
}


