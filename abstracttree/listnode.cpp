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

QString ListNode::printTripleCode()
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
            else {
                if(_left->isValueNode())
                    outString.append(((AbstractValueASTNode *) _left)->getValueTypeLLVM());
                outString.append(" " + _left->printTripleCode());
            }

        }


        if(_right != NULL) {
            outString.append(", ");
            if(_right->getType() == NT_List) {
                ((ListNode *) _right)->setListType(LT_CallParamList);
                outString.append(_right->printTripleCode());
            }
            else {
                if(_right->isValueNode())
                    outString.append(((AbstractValueASTNode *) _right)->getValueTypeLLVM());
                outString.append(" " + _right->printTripleCode());
            }
        }
        break;
    case LT_CaseValueList:
        //print goto
        if(_left != NULL) {
            if(_left->getType() == NT_List) {
                ((ListNode *) _left)->setListType(LT_CaseValueList);
                _left->printTripleCode();
            }
            else {
                ((CaseNode *) _left)->printValues();
            }
        }


        if(_right != NULL) {
            if(_right->getType() == NT_List) {
                ((ListNode *) _right)->setListType(LT_CaseValueList);
                _right->printTripleCode();
            }
            else {
                ((CaseNode *) _right)->printValues();
            }
        }
        break;
    case LT_CaseList:
        if(_left != NULL) {
            if(_left->getType() == NT_List) {
                ((ListNode *) _left)->setListType(LT_CaseList);
            }
            _left->printTripleCode();
        }


        if(_right != NULL) {
            if(_right->getType() == NT_List) {
                ((ListNode *) _right)->setListType(LT_CaseList);
            }
            _right->printTripleCode();
        }
        break;
    case LT_ArrayValueList:
        break;
    default:
        break;
    }

    return outString;
}
void ListNode::setSize(){
    if(_typeList == LT_ArrayValueList){
        AbstractASTNode* tmp;
        tmp = this->getLeftNode();
        while(tmp != NULL) {
                    if(tmp->getType() == NT_List) {
                        ((ListNode *) tmp)->setListType(LT_ArrayValueList);
                         tmp = ((ListNode *)tmp)->getLeftNode();
                     }
                    else {
                        if(tmp->isValueNode()) {
                            //outString.append(((AbstractValueASTNode *) _left)->getValueTypeLLVM());
                        //outString.append(" " + _left->printTripleCode());
                       // qDebug()<<_size;
                        }
                        tmp = NULL;
                    }
                    _size++;
                // qDebug()<<_size;
                }

        tmp = this->getRightNode();
                while(tmp != NULL) {

                    if(tmp->getType() == NT_List) {
                        ((ListNode *) tmp)->setListType(LT_ArrayValueList);
                        tmp = ((ListNode *)tmp)->getRightNode();
                        //qDebug()<<_size;


                    }
                    else {
                        if(tmp->isValueNode()){
                       //     outString.append(((AbstractValueASTNode *) _right)->getValueTypeLLVM());
                        //outString.append(" " + _right->printTripleCode());
                        //qDebug()<<_size;
                        }
                        tmp = NULL;
                    }

                     _size++;
                     //qDebug()<<_size;
                }
    }
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

