#ifndef GOTONODE_H
#define GOTONODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The GoToNode class
 */
class GoToNode : public AbstractASTNode
{
public:
    GoToNode(QString nameLabel, QString key);

    void printNode(int level);

    QString printTripleCode(int level);

    ~GoToNode();

private:
    QString _nameLabel;
    QString _key;
};

#endif // GOTONODE_H
