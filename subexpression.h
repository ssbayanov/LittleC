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
    typeFunction,
    typeString
} ValueTypeEnum;

static QStringList typeName = QStringList() << "int"
                                            << "short"
                                            << "float"
                                            << "double"
                                            << "label"
                                            << "void"
                                            << "char"
                                            << "bool"
                                            << "struct"
                                            << "function"
                                            << "string";
#endif // SUBEXPRESSION_H

