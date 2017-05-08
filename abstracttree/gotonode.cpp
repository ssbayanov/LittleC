#include "gotonode.h"

GoToNode::GoToNode(QString nameLabel)
    : AbstractASTNode(NT_GoToStatement)
{
    _nameLabel = nameLabel;
}

void GoToNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Go to label: %1\n").arg(_nameLabel);
}

QString GoToNode::printTripleCode()
{
    if(!_nameLabel.contains("$")) {
        ir.writeLine(QString("br label %1")
                     .arg(ir.getUserLabelByName(_nameLabel)));
    }
    else {
        ir.writeLine(QString("br label %1")
                     .arg(_nameLabel));
    }
    return "";
}

GoToNode::~GoToNode()
{

}

