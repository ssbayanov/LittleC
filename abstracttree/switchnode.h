#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The SwitchNode class
 */
class SwitchNode : public AbstractASTNode
{
public:
    /**
     * @brief SwitchNode construcotr of class
     * @param value pointer to value node
     * @param caseList pointer to cases list node
     */
    SwitchNode(AbstractASTNode *value, AbstractASTNode *caseList);

    /**
     * @brief printNode printing switch node to stdout
     * @param level
     */
    void printNode(int level);

    /**
      * @brief ~SwitchNode destruction class and call destrucor of _value and _caseList
      */
    ~SwitchNode();

private:
    /**
     * @brief _value pointer to switching value or expression
     */
    AbstractASTNode *_value;

    /**
     * @brief _caseList pointer to cases list
     */
    AbstractASTNode *_caseList;

};

#endif // SWITCHNODE_H
