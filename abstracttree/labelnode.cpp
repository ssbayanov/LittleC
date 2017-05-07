#include "labelnode.h"

LabelNode::LabelNode(AbstractSymbolTableRecord *variable)
    : AbstractASTNode(NT_LabelStatement)
{
    _variable = variable;
}

void LabelNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
              << QString("Label: %1\n")
                 .arg(_variable->getName());
}

QString LabelNode::printTripleCode(int , QString )
{
//    ir.writeLine( QString("; <label>:%1")
//                 .arg(_variable->getName()));


    ir.writeUserLabelLine(_variable->getName());

    return "";
}



LabelNode::~LabelNode()
{

}
