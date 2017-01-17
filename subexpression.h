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
    typeEnum,      /* Floating point number with double precision */
    typeFunction
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
                                            << "enum"
                                            << "function";
#endif // SUBEXPRESSION_H

