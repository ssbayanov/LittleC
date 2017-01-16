#include "labelnode.h"

LabelNode::LabelNode(AbstractSymbolTableRecord *variable)
    : AbstractASTNode(NT_LabelStatement)
{
    _variable = variable;
}

void LabelNode::printNode(int level)
{
    std::cout << QString().fill(' ',level*2).toStdString()
              << QString("Label: %1").arg(_variable->getName()).toStdString()
              << std::endl;
}

LabelNode::~LabelNode()
{

}
