#include "labelnode.h"

LabelNode::LabelNode(AbstractSymbolTableRecord *variable)
    : AbstractASTNode(NT_LabelStatement)
{
    _variable = variable;
}

void LabelNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Label: %1\n").arg(_variable->getName());
}

LabelNode::~LabelNode()
{

}
