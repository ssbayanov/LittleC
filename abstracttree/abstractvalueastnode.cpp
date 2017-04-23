#include "abstractvalueastnode.h"

AbstractValueASTNode::AbstractValueASTNode(ASTNodeTypeEnum type)
    : AbstractASTNode(type)
{

}

ValueTypeEnum AbstractValueASTNode::getValueType()
{
    return _typeValue;
}

AbstractValueASTNode::~AbstractValueASTNode()
{

}

bool AbstractValueASTNode::isValueNode()
{
    return true;
}

QString AbstractValueASTNode::getValueTypeLLVM()
{
    switch(_typeValue) {
    case typeVoid:
        return "void";
    case typeBool:
    case typeChar:
    case typeShort:
        return "i8";
    case typeFloat:
        return "float";
    case typeInt:
        return "i32";
    case typeDouble:
        return "double";
    default:
        return "undef";
    }
}

