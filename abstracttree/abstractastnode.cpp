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
    treeStream << QString("printing Abstract node :( on level %1\n").arg(level);
}

QString AbstractASTNode::printTripleCode(int level, QString param)
{
    outStream << QString("printing Abstract node :( on level %1\n").arg(level);
}

AbstractASTNode::~AbstractASTNode()
{

}

bool AbstractASTNode::isValueNode()
{
    return false;
}
