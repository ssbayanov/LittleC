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

int getSizeType(ValueTypeEnum type);


int isInt(ValueTypeEnum type);

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

