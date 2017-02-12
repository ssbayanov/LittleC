#include "abstractastnode.h"

AbstractASTNode::AbstractASTNode(ASTNodeTypeEnum type)
{
    _nodetype = type;
}

void AbstractASTNode::setType(ASTNodeTypeEnum type)
{
    _nodetype = type;
}

ASTNodeTypeEnum AbstractASTNode::getType()
{
    return _nodetype;
}

void AbstractASTNode::printNode(int level)
{
    std::cout << QString("printing Abstract node :( on level %1").arg(level).toStdString();
}

AbstractASTNode::~AbstractASTNode()
{

}
