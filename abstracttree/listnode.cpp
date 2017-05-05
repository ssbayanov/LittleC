#include "listnode.h"
#include "variabledeclarationnode.h"

ListNode::ListNode(AbstractASTNode *left, AbstractASTNode *right, ListTypeEnum typeList)
    : AbstractASTNode(NT_List)
{
    _left = left;
    _right = right;
    _typeList = typeList;
}

void ListNode::printNode(int level)
{
    treeStream << QString().fill(' ',level*2)
               << "List:\n";
    if(_left != NULL){
        treeStream << QString().fill(' ',level*2)
                   << "Left:\n";
        _left->printNode(level+1);
    }
    else {
        treeStream  << QString().fill(' ',level*2)
                    << "BAD LEFT LIST NODE\n";
        ;
    }

    if(_right != NULL) {
        treeStream << QString().fill(' ',level*2)
                   << "Right:\n";
        _right->printNode(level+1);
    }
    else
        treeStream << "BAD RIGHT LIST NODE";

}

QString ListNode::printTripleCode(int level, QString param)
{
    QString outString = "";

    switch (_typeList) {
    case LT_Default:
        if(_left != NULL) {

            _left->printTripleCode();
        }
        if(_right != NULL) {

            _right->printTripleCode();
        }
        break;
    case LT_DeclareParamList:

        if(_left != NULL) {
            if(_left->getType() == NT_Declaration) {
                outString = ((VariableDeclarationNode *) _left)->printParamTriple();
            }
            else {
                outString = _left->printTripleCode();
            }

        }
        if(_right != NULL) {
            outString.append(", ");
            if(_right->getType() == NT_Declaration) {

                outString.append(((VariableDeclarationNode *) _right)->printParamTriple());
            }
            else {
                outString.append(_right->printTripleCode());
            }
        }
        break;

    case LT_CallParamList:
        if(_left != NULL) {
            if(_left->getType() == NT_List) {
                ((ListNode *) _left)->setListType(LT_CallParamList);
                outString.append(_left->printTripleCode());
            }
            if(_left->isValueNode())
                outString.append(((AbstractValueASTNode *) _left)->getValueTypeLLVM());
           outString.append(" " + _left->printTripleCode());

        }


        if(_right != NULL) {
            outString.append(", ");
            if(_right->getType() == NT_List) {
                ((ListNode *) _right)->setListType(LT_CallParamList);
                outString.append(_right->printTripleCode());
            }
            if(_right->isValueNode())
                outString.append(((AbstractValueASTNode *) _right)->getValueTypeLLVM());
            outString.append(" " + _right->printTripleCode());
        }
        break;
    default:
        break;
    }

    return outString;
}

AbstractASTNode *ListNode::getLeftNode()
{
    return _left;
}

AbstractASTNode *ListNode::getRightNode()
{
    return _right;
}

void ListNode::setLeftNode(AbstractASTNode *left)
{
    _left = left;
}

void ListNode::setRightNode(AbstractASTNode *right)
{
    _right = right;
}

void ListNode::setListType(ListTypeEnum typeList)
{
    _typeList = typeList;
}

ListNode::~ListNode()
{
    if(_left != NULL)
        _left->~AbstractASTNode();
    if(_right != NULL)
        _right->~AbstractASTNode();
}

