#ifndef BINARNODE_H
#define BINARNODE_H

#include "abstracttree/abstractvalueastnode.h"
/**
 * @brief The BinarNode class
 */

typedef enum {
    BO_Undef,
    BO_Assign,
    BO_Add,
    BO_Sub,
    BO_Mul,
    BO_Div,
    BO_Rem,
    BO_And,
    BO_AndBitwise,
    BO_Or,
    BO_OrBitwise,
    BO_Xor,
    BO_Lees,
    BO_LeesEq,
    BO_Eq,
    BO_GreatEq,
    BO_Great,
    BO_NotEq
} BinarNodeOperationEnum;

static QStringList BinarNodeOperationText
= QStringList() << "UNDEF"
                << "="
                << "+"
                << "-"
                << "*"
                << "/"
                << "%"
                << "&&"
                << "&"
                << "||"
                << "|"
                << "^"
                << "<"
                << "<="
                << "=="
                << ">="
                << ">"
                << "!=";

class BinarNode : public AbstractValueASTNode
{
public:
    BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation);

    BinarNode(AbstractASTNode *left, AbstractASTNode *right, QString operation, ValueTypeEnum typeValue);



    virtual QString printTripleCode();


    void printNode(int level);

    ~BinarNode();

private:
    AbstractASTNode *_left;
    AbstractASTNode *_right;
    BinarNodeOperationEnum _operation;
    QString _operationText;

    BinarNodeOperationEnum getOperation(QString op);
};


#endif // BINARNODE_H
