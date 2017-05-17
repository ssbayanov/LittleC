#include "parsersupport.h"

void yyerror(QString s) {
    if (printErrors)
        printf ("Line %d: %s.\n", lno, s.toStdString().c_str());
}

AbstractSymbolTableRecord *getVariableByName(QString name) {
    AbstractSymbolTableRecord *var = currentTable->getVariableGlobal(name);

    if (var == NULL)
    {
        parsererror(errorList.at(ERROR_NOT_DECLARED).arg(name));
        //YYERROR;
    }
    return var;
}

bool isNumericType( ValueTypeEnum type )
{
    if (type == typeInt ||
            type == typeBool ||
            type == typeDouble ||
            type == typeFloat ||
            type == typeShort ||
            type == typeChar)
        return true;

    return false;
}

bool isNumericType( AbstractValueASTNode *node )
{
    return isNumericType( ((AbstractValueASTNode *)node)->getValueType() );
}

bool contains(QString name)
{
    if ( !currentTable->contains(name) ) {
        if (currentTable->getVariableGlobal(name) != NULL)
        {
            parsererror(errorList.at(WARNING_DOUBLE_DECLARED).arg(name));
        }
        return false;
    }
    parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(name));
    return true;

}


AbstractASTNode *binarMathNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if ( isNumericType(left) && isNumericType(right) ) {

        if (left->getValueType() != right->getValueType())
        {
            if (isInt(left->getValueType())) {
                if(isInt(right->getValueType())){
                    if(getSizeType(left->getValueType()) > getSizeType(right->getValueType())) {
                        right = convert(right, left->getValueType());
                    }
                    else {
                        left = convert(left, right->getValueType());
                    }
                }
                else {
                    left = convert(left, right->getValueType());
                }
            }
            else {
                if(isInt(right->getValueType())){
                    right = convert(right, left->getValueType());
                }
                else {
                    if(getSizeType(left->getValueType()) > getSizeType(right->getValueType())) {
                        right = convert(right, left->getValueType());
                    }
                    else {
                        left = convert(left, right->getValueType());
                    }
                }
            }
        }
        if (right == NULL || left == NULL)
            return NULL;
        else
            return new BinarNode(left, right, operation);
    }
    else {
        parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE).arg(typeName.at(left->getValueType())).arg(typeName.at(right->getValueType())));
        return NULL;
    }
}

AbstractASTNode *binarBoolNode(AbstractValueASTNode *left, QString operation, AbstractValueASTNode *right)
{
    if ( isNumericType(left) && isNumericType(right) ) {
        if (operation == "&&" || operation == "||") {
            if (left->getValueType() != typeBool)
                left = new UnaryNode(UnarToBool, left);
            if (right->getValueType() != typeBool)
                right = new UnaryNode(UnarToBool, right);
        }

        return new BinarNode(left, right, operation, typeBool);
    }
    else {
        if ( !isNumericType(left) ) {
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(left->getValueType()))
                        .arg(typeName.at(typeBool))); }
        else {
            if(!isInt(left->getValueType())) {
                left = convert(left, typeInt);
                if(left == NULL) {
                    parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
                    return NULL;
                }

            }
        }

        if ( !isNumericType(right) ) {
            parsererror(errorList.at(ERROR_CANNOT_CONVERT)
                        .arg(typeName.at(right->getValueType()))
                        .arg(typeName.at(typeBool))); }
        else {
            if(!isInt(right->getValueType())) {
                right = convert(right, typeInt);
                if(right == NULL) {
                    parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
                    return NULL;
                }

            }
        }

        return new BinarNode(left, right, operation, typeBool);

    }
}

AbstractASTNode *numericDeclaration(ValueTypeEnum type, QString name, AbstractValueASTNode *value)
{
    if ( !contains(name) ) {
        AbstractSymbolTableRecord *var = currentTable->insertVariable(name, type, 0);
        if (var == NULL) {
            parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
            return NULL;
        }
        if(value != NULL) {
            if(value->getValueType() != type) {
                value = convert(value, type);
                if (value == NULL) {
                    parsererror(errorList.at(ERROR_MEMORY_ALLOCATION));
                    return NULL;
                }
            }
        }
        return new VariableDeclarationNode(var, value);
    }
    else {
        parsererror(errorList.at(ERROR_DOUBLE_DECLARED).arg(name));
        return NULL;
    }
}

AbstractValueASTNode *convert(AbstractValueASTNode *what, ValueTypeEnum to)
{
    ValueTypeEnum whatType = what->getValueType();
    if (isNumericType(whatType)) {
        if (isNumericType(to)) {
            switch(to){
            case typeBool:
                return new UnaryNode(UnarToBool, what);
            case typeChar:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToChar, what);
            case typeDouble:
                return new UnaryNode(UnarToDouble, what);;
            case typeFloat:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToFloat, what);
            case typeInt:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToInt, what);
            case typeShort:
                if (getSizeType(whatType) > getSizeType(to))
                    parsererror(errorList.at(WARNING_CONVERTING_TYPES)
                                .arg(typeName.at(whatType))
                                .arg(typeName.at(to)));
                return new UnaryNode(UnarToShort, what);
            default:
                parsererror(errorList.at(ERROR_UNKNOWN_TYPE).arg(to));
                return NULL;
            }
        }
    }
    parsererror(errorList.at(ERROR_TYPES_INCOMPATIBLE)
                .arg(typeName.at(what->getValueType()))
                .arg(typeName.at(to)));
    return NULL;

}

AbstractASTNode *validatedParams(FunctionTableRecord *function, AbstractASTNode *paramsNode)
{
    QList<ValueTypeEnum> callParamsTypes;
    AbstractASTNode *currentNode = paramsNode;
    AbstractASTNode *lastListNode = NULL;
    SymbolsTable::iterator pI = function->getParams()->begin();

    QString paramsString = "";

    if(currentNode != NULL) {
        // Bypass list
        while ( !currentNode->isValueNode() ) {
            AbstractValueASTNode *valueNode;

            if (currentNode->getType() == NT_List) {
                if (((ListNode *) currentNode)->getLeftNode()->isValueNode()) {
                    valueNode = ((AbstractValueASTNode *)((ListNode *) currentNode)->getLeftNode());
                }
                else {
                    parsererror(errorList.at(ERROR_PARAMS_FUNCTION).arg(function->getName()));
                    return NULL;
                }
            }
            else {
                parsererror(errorList.at(ERROR_PARAMS_FUNCTION).arg(function->getName()));
                return NULL;
            }

            if (pI != function->getParams()->end()) {
                if (valueNode->getValueType() !=  (*pI)->getValueType()) {
                    valueNode = convert(valueNode, (*pI)->getValueType());
                    if (valueNode != NULL) {
                        ((ListNode *) currentNode)->setLeftNode(valueNode);
                    }
                    else {
                        //error printing in convert function
                        return NULL;
                    }
                }
            }

            callParamsTypes << valueNode->getValueType();
            if(paramsString.length() != 0)
                paramsString.append(",");
            paramsString.append(typeName.at(valueNode->getValueType()));

            lastListNode = currentNode;
            currentNode = ((ListNode *) currentNode)->getRightNode();

            if (pI != function->getParams()->end()) {
                ++pI;
            }
        }
        if (pI != function->getParams()->end()) {
            if (((AbstractValueASTNode *) currentNode)->getValueType() !=  (*pI)->getValueType()) {
                currentNode = convert(((AbstractValueASTNode *) currentNode), (*pI)->getValueType());
                if (currentNode != NULL) {
                    if( lastListNode != NULL ) {
                        ((ListNode *) lastListNode)->setRightNode(currentNode);
                    }
                    else {
                        // if one parametr and converting success returning there
                        return currentNode;
                    }
                }
                else {
                    //error printing in convert function
                    return NULL;
                }
            }
        }
        callParamsTypes << ((AbstractValueASTNode *) currentNode)->getValueType();

        if(paramsString.length() != 0)
            paramsString.append(",");
        paramsString.append(typeName.at(((AbstractValueASTNode *) currentNode)->getValueType()));
    }

    if (callParamsTypes.count() == function->getParams()->count()) {
        pI = function->getParams()->begin();
        for(int i = 0; i < callParamsTypes.count(); i++, pI++) {
            if (callParamsTypes.at(i) != (*pI)->getValueType())
            {
                parsererror(errorList.at(ERROR_CALL_UNDEFINED_FUNCTION_WITH_PARAMS).arg(function->getName()).arg(paramsString));
                return NULL;
            }
        }
        return paramsNode;
    }
    else {
        parsererror(errorList.at(ERROR_CALL_UNDEFINED_FUNCTION_WITH_PARAMS).arg(function->getName()).arg(paramsString));
    }

    return NULL;
}
