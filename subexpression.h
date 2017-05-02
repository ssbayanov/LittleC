#ifndef SUBEXPRESSION_H
#define SUBEXPRESSION_H

#include <QStringList>

typedef enum
{
    typeInt,
    typeShort,
    typeFloat,
    typeDouble,
    typeLabel,
    typeVoid,
    typeChar,
    typeBool,
    typeStruct,
    typeStructType,
    typeFunction,
    typeString
} ValueTypeEnum;

static int getSizeType(ValueTypeEnum type)
{
    switch(type) {
    case typeVoid:
        return 0;
    case typeBool:
    case typeChar:
        return 1;
    case typeShort:
        return 2;
    case typeFloat:
    case typeInt:
        return 4;
    case typeDouble:
        return 8;
    default:
        return -1;
    }
}

static int isInt(ValueTypeEnum type)
{
    if(type == typeBool
            || type == typeInt
            || type == typeShort
            || type == typeChar)
        return true;
    return false;
}

static QStringList typeName = QStringList() << "int"
                                            << "short"
                                            << "float"
                                            << "double"
                                            << "label"
                                            << "void"
                                            << "char"
                                            << "bool"
                                            << "struct"
                                            << "struct type"
                                            << "function"
                                            << "string";
#endif // SUBEXPRESSION_H

