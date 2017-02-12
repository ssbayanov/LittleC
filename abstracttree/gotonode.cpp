#include "gotonode.h"

GoToNode::GoToNode(QString nameLabel)
    : AbstractASTNode(NT_GoToStatement)
{
    _nameLabel = nameLabel;
}

void GoToNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Go to label: %1").arg(_nameLabel).toStdString()
              << std::endl;
}

GoToNode::~GoToNode()
{

}

