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
    treeStream << QString("printing Abstract node. Child class has not member printNode(int). Level: %1. Type: %2\n")
                  .arg(level)
                  .arg(_nodetype);
}

QString AbstractASTNode::printTripleCode(int level, QString param)
{
    outStream << QString("printing Abstract node. Child class has not member printTripleCode(int, QString). Level: %1 Type: %2\n")
                 .arg(level)
                 .arg(_nodetype);
}

AbstractASTNode::~AbstractASTNode()
{

}

bool AbstractASTNode::isValueNode()
{
    return false;
}
