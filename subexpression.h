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
    typeMass,
    typeEnum      /* Floating point number with double precision */
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
                                            << "mass"
                                            << "enum";
#endif // SUBEXPRESSION_H

