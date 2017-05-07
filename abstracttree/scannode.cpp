#include "scannode.h"

ScanNode::ScanNode(ValueTypeEnum typeValue)
    : AbstractValueASTNode(NT_ScanExpression)
{
    _typeValue = typeValue;
}

void ScanNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Scan, type %1:\n")
                 .arg(typeName.at(getValueType()));
}

QString ScanNode::printTripleCode()
{
    ir.addExternFunction("declare i32 @scanf(i8*, ...)");

    QString formatStringGet = "i8* getelementptr inbounds ([%2 x i8], [%2 x i8]* %1, i32 0, i32 0)";

    QString ptrInputValue = ir.writeCommandLine(QString("alloca %1")
                                                .arg(getValueTypeLLVM()));

    QString stringType = "";
    switch (getValueType()) {
    case typeInt:
    case typeBool:
    case typeShort:
        stringType = "%d";
        break;
    case typeFloat:
        stringType = "%f";
        break;
    case typeDouble:
        stringType = "%lf";
        break;
    default:
        break;
    }

    QString ptrFormatString = ir.addUnnamedVariable(
                    QString("private unnamed_addr constant [%1 x i8] c\"%2\\00\"")
                    .arg(stringType.length()+1)
                    .arg(stringType
                         .replace("\n","\\0A")
                         .replace("\t", "\\09")));

    formatStringGet = formatStringGet
            .arg(ptrFormatString)
            .arg(stringType.length()+1);

    ir.writeCommandLine(QString("call i32 (i8*, ...) @scanf(%1,%3* %2)")
                        .arg(formatStringGet)
                        .arg(ptrInputValue)
                        .arg(getValueTypeLLVM()));

    QString outValue = ir.writeCommandLine(QString("load %1, %1* %2")
                                           .arg(getValueTypeLLVM())
                                           .arg(ptrInputValue));

    return outValue;

}

ScanNode::~ScanNode()
{

}

