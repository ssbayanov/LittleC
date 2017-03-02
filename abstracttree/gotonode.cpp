#include "gotonode.h"

GoToNode::GoToNode(QString nameLabel, QString key)
    : AbstractASTNode(NT_GoToStatement)
{
    _nameLabel = nameLabel;
    _key = key;
}

void GoToNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Go to label: %1\n").arg(_nameLabel);
}

QString GoToNode::printTripleCode(int level)
{
    outStream << QString("goto %1_%2\n").arg(_nameLabel).arg(_key);
    return "";
}

GoToNode::~GoToNode()
{

}

