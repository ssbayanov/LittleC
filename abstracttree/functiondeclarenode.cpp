#include "functiondeclarenode.h"
#include "variabledeclarationnode.h"

FunctionDeclareNode::FunctionDeclareNode(AbstractSymbolTableRecord *variable, AbstractASTNode *paramList, AbstractASTNode *body)
    : AbstractValueASTNode(NT_FunctionDeclare)
{
    _variable = variable;
    _paramList = paramList;
    _body = body;
    _typeValue = variable->getValueType();
}

void FunctionDeclareNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << QString("Declaration function: %1, type: %2\n")
                  .arg(_variable->getName())
                  .arg(typeName.at(_variable->getValueType()));

    if (_paramList != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Params:\n";
        _paramList->printNode(level+1);
    }

    if (_body != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Body:\n";
        _body->printNode(level+1);
    }
    else {
        treeStream << QString().fill(' ',level*2)
                   << "BAD BODY NODE!!!\n";
    }
}

QString FunctionDeclareNode::printTripleCode()
{
    QString params = "";

    ir.startStore();

    ir.resetCommandCounter();

    ir.writeLine(QString("define %2 @%1 (%params) {\n")
                 .arg(_variable->getName())
                 .arg(getValueTypeLLVM()));

    if(_paramList != NULL) {
        if(_paramList->getType() == NT_Declaration) {
            params = ((VariableDeclarationNode *) _paramList)->printParamTriple();
        }
        else {
            params = _paramList->printTripleCode();
        }
    }

    if(_body != NULL){
        _body->printTripleCode();
    }

    ir.writeLine( QString("}\n"));

    ir.replaceInStored("%params", params);

    ir.flushStore();

    return "";
}

FunctionDeclareNode::~FunctionDeclareNode()
{
    if (_paramList != NULL)
        _paramList->~AbstractASTNode();
    if (_body != NULL)
        _body->~AbstractASTNode();
}


