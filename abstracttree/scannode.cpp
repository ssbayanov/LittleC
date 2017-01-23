#include "scannode.h"

ScanNode::ScanNode(ValueTypeEnum typeValue)
    : AbstractValueASTNode(NT_ScanExpression)
{
    _typeValue = typeValue;
}

void ScanNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Scan, type %1:\n")
                 .arg(typeName.at(getValueType()));
}

ScanNode::~ScanNode()
{

}

