#ifndef GOTONODE_H
#define GOTONODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The GoToNode class
 */
class GoToNode : public AbstractASTNode
{
public:
    GoToNode(QString nameLabel);

    void printNode(int level);

    ~GoToNode();

private:
    QString _nameLabel;
};

#endif // GOTONODE_H
