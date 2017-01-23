#ifndef SCANNODE_H
#define SCANNODE_H

#include "abstracttree/abstractvalueastnode.h"
/**
 * @brief The BinarNode class
 */

class ScanNode : public AbstractValueASTNode
{
public:
    ScanNode(ValueTypeEnum typeValue);

    void printNode(int level);

    ~ScanNode();
};

#endif // SCANNODE_H
