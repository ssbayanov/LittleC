#ifndef ABSTRACTVALUEASTNODE_H
#define ABSTRACTVALUEASTNODE_H

#include "abstracttree/abstractastnode.h"

/**
 * @brief The AbstractValueASTNode class This class using for descripted variable
 * or other construction wich has value.
 */
class AbstractValueASTNode  : public AbstractASTNode
{
public:
    /**
     * @brief AbstractValueASTNode Construction of class.
     * @param type Type of node.
     */
    AbstractValueASTNode(ASTNodeTypeEnum type);

    /**
     * @brief ~AbstractValueASTNode Destruction of class
     */
    virtual ~AbstractValueASTNode();

    /**
     * @brief isValueNode Verification is AbstractrVlueNode
     * @return Always return true
     */
    bool isValueNode();

    /**
     * @brief getValueType Get type of value.
     * @return Value of _typeValue.
     */
    virtual ValueTypeEnum getValueType();

    /**
     * @brief getValueTypeLLVM Get value type of LLVM IR notation.
     * @return String of an type in LLVM IR notation.
     */
    QString getValueTypeLLVM();

    /**
     * @brief getValueTypeLLVM Get value type of LLVM IR notation.
     * @param type Input type.
     * @return String of an type in LLVM IR notation.
     */
    static QString getValueTypeLLVM(ValueTypeEnum type);

protected:

    /**
     * @brief _typeValue Contains type of value.
     */
    ValueTypeEnum _typeValue;
};
#endif // ABSTRACTVALUEASTNODE_H
