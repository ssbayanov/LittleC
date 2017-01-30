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

