#include "abstractvalueastnode.h"

AbstractValueASTNode::AbstractValueASTNode(ASTNodeTypeEnum type)
    : AbstractASTNode(type)
{

}

ValueTypeEnum AbstractValueASTNode::getType()
{
    return _typeValue;
}

AbstractValueASTNode::~AbstractValueASTNode()
{

}

