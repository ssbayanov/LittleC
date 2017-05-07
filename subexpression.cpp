#include "subexpression.h"


int getSizeType(ValueTypeEnum type)
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

int isInt(ValueTypeEnum type)
{
    if(type == typeBool
            || type == typeInt
            || type == typeShort
            || type == typeChar)
        return true;
    return false;
}
